#include "engine.hpp"


Engine::Engine() : status(acting), con(71, 38), img("GameOver.png")
{
	TCODConsole::initRoot(80, 43, "Ven L' King");
}


Engine::~Engine()
{
	if (map) delete map;
}

void Engine::term()
{
	actors.clearAndDelete();
	delete player;
	if (map) delete map;
}

void Engine::init()
{

	player = new Actor(40, 25,true, '@', "Player", TCODColor::white);
	player->ai = new PlayerAi();
	player->destructible = new Destructible(100,20,10,"Corpse");
	player->attacker = new Attacker(50);
	player->container = new Container(29);

	stairs = new Actor(0, 0, false, '>', "stairs", TCODColor::white);
	stairs->fovOnly = false;
	actors.push(stairs);

	map = new Map(67, 36);
	map->init(true);
	map->computeFov();
}

void Engine::render()
{
	TCODConsole::root->clear();
	if(status != menu && status != victory && status != death){
		con.clear();
		if (status == acting) 	map->computeFov();
		map->render();
		for (auto const& a : actors) {
			if (a != player
				&& ((!a->fovOnly && map->isExplored(a->x, a->y))
					|| map->isInFov(a->x, a->y))) {
				a->render();
			}
		}
		player->render();

		TCODConsole::blit(&con, 0, 0, 67, 36, TCODConsole::root, 0, 7);

		TCODConsole::root->setDefaultForeground(TCODColor::lightCyan);
		TCODConsole::root->printFrame(67, 0, 13, 7);
		TCODConsole::root->setDefaultForeground(TCODColor::green);
		TCODConsole::root->print(68, 1, "Player");
		TCODConsole::root->setDefaultForeground(TCODColor::white);
		if (player->destructible) TCODConsole::root->print(68, 3, "HP: %d/%d", player->destructible->curHP, player->destructible->maxHP);
		if(player->attacker) TCODConsole::root->print(68, 4, "AP: %d", player->attacker->AP);
	
		if(player->destructible) TCODConsole::root->print(68, 5, "DP: %d", player->destructible->DP);

		TCODConsole::root->setDefaultForeground(TCODColor::red);
		TCODConsole::root->printFrame(67, 7, 13, 35);
		TCODConsole::root->setDefaultForeground(TCODColor::green);
		TCODConsole::root->print(68, 9, "Inventory %d", player->container->inventory.size());

		if(player->container)
		for (int i = 0; i < player->container->inventory.size(); i++) {
			TCODConsole::root->setDefaultForeground(player->container->inventory.get(i)->col);
			TCODConsole::root->print((68)+((11 - player->container->inventory.get(i)->name.size())/2), i + 11, player->container->inventory.get(i)->name.c_str());
		}
		TCODConsole::root->setDefaultForeground(TCODColor::white);
		TCODConsole::root->printFrame(0, 0, 67, 7);

		for (int i = 0; i < 5; i++) {
			if(log.size() > i){
				TCODConsole::root->setDefaultForeground(log.get(log.size() - i - 1)->col);
				TCODConsole::root->print((65/2) - (log.get(log.size() - i - 1)->text.size()/2), i + 1, log.get(log.size() - i - 1)->text.c_str());
			}
		}

		if(player->ai) player->ai->renderGUI(player);

		for (auto const& a : actors) {
			if (a->ai != NULL) {
				a->ai->renderGUI(a);
			}
		}
	}
	else if (status == death) {
		img.blit2x(TCODConsole::root, 0, 0);
	}
}

void Engine::update()
{
	lastKey = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
	if (status != menu && status != victory && status != death) {
		
		if (status == startup) map->computeFov();
		status = idle;

		player->update();

		if (status == acting) {
			for (auto const& a : actors) {
				a->update();
			}
		}
		if (!player->destructible) {
			status = death;
		}
	}
	else if(status == death){
		if (lastKey.c == 'y'){
			term();
			init();
		}
		else if (lastKey.c == 'n') {
			term();
		}
	}
}

void Engine::nextLevel()
{
	level++;
	player->destructible->healing(player->destructible->maxHP / 2);
	delete map;
	// delete all actors but player and stairs
	for (Actor **it = actors.begin(); it != actors.end(); it++) {
		if (*it != player && *it != stairs) {
			delete *it;
			it = actors.remove(it);
		}
	}

	log.clear();

	map = new Map(67, 36);
	map->init(true);

	status = startup;
}

void Engine::addMsg(Message * text)
{
	log.push(text);
}

Message::Message(std::string text, const TCODColor & col) : text(text), col(col)
{

}

Message::~Message()
{
	text.clear();
}
