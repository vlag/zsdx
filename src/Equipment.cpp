#include "Equipment.h"
#include "ZSDX.h"
#include "Game.h"
#include "Savegame.h"
#include "entities/Link.h"

/**
 * Constructor.
 */
Equipment::Equipment(Savegame *savegame):
  savegame(savegame), magic_decrease_delay(0) {
}

/**
 * Destructor.
 */
Equipment::~Equipment(void) {

}

/**
 * This function is be called repeatedly by the game.
 * Most of the time, there is nothing to update in this
 * class. The only element updated here is the magic bar
 * when it decreases continuously.
 */
void Equipment::update(void) {

  if (magic_decrease_delay != 0) {
    // the magic bar is decreasing

    Uint32 ticks = SDL_GetTicks();
    if (!zsdx->game->is_suspended()) {

      if (ticks > next_magic_decrease_date) {
	
	remove_magic(1);
	
	if (get_magic() > 0) {
	  next_magic_decrease_date += magic_decrease_delay;
	}
	else {
	  stop_removing_magic();
	}
      }
    }
    else {
      // delay the next decrease while the game is suspended
      next_magic_decrease_date = SDL_GetTicks();
    }
  }
}

// tunic

/**
 * Returns the number of Link's tunic.
 * @return Link's tunic number (0: green tunic,
 * 1: blue tunic, 2: red tunic)
 */
int Equipment::get_tunic_number(void) {
  return savegame->get_reserved_integer(Savegame::LINK_TUNIC);
}

/**
 * Sets Link's tunic.
 * The savegame data is modified.
 * @param tunic_number Link's tunic number (0: green tunic,
 * 1: blue tunic, 2: red tunic)
 */
void Equipment::set_tunic_number(int tunic_number) {

  if (tunic_number != get_tunic_number()) {
    // the tunic has changed

    if (tunic_number < 0 || tunic_number > 2) {
      DIE("Illegal tunic number: " << tunic_number);
    }

    savegame->set_reserved_integer(Savegame::LINK_TUNIC, tunic_number);

    if (zsdx->game != NULL) {
      zsdx->game->get_link()->initialize_sprites(); // reinitialize Link's sprites
    }
  }
}

// sword

/**
 * Returns whether Link has a sword.
 * @return true if Link has a sword, i.e. if get_sword_number() > 0
 */
bool Equipment::has_sword(void) {
  return get_sword_number() > 0;
}

/**
 * Returns the number of Link's sword.
 * @return Link's sword number (0: no sword,
 * 1 to 4: sword 1 to 4)
 */
int Equipment::get_sword_number(void) {
  return savegame->get_reserved_integer(Savegame::LINK_SWORD);
}

/**
 * Sets Link's sword.
 * The savegame data is modified.
 * @param sword_number Link's sword number (0: no sword,
 * 1 to 4: sword 1 to 4)
 */
void Equipment::set_sword_number(int sword_number) {

  if (sword_number != get_sword_number()) {
    // the sword has changed

    if (sword_number < 0 || sword_number > 4) {
      DIE("Illegal sword number: " << sword_number);
    }

    savegame->set_reserved_integer(Savegame::LINK_SWORD, sword_number);

    if (zsdx->game != NULL) {
      zsdx->game->get_link()->initialize_sprites(); // reinitialize Link's sprites
    }
  }
}

// shield

/**
 * Returns whether Link has a shield.
 * @return true if Link has a shield, i.e. if get_shield_number() > 0
 */
bool Equipment::has_shield(void) {
  return get_shield_number() > 0;
}

/**
 * Returns the number of Link's shield.
 * @return Link's shield number (0: no shield,
 * 1 to 3: shield 1 to 3)
 */
int Equipment::get_shield_number(void) {
  return savegame->get_reserved_integer(Savegame::LINK_SHIELD);
}

/**
 * Sets Link's shield.
 * The savegame data is modified.
 * @param shield_number Link's shield number (0: no shield,
 * 1 to 3: shield 1 to 3)
 */
void Equipment::set_shield_number(int shield_number) {

  if (shield_number != get_shield_number()) {
    // the shield has changed

    if (shield_number < 0 || shield_number > 3) {
      DIE("Illegal shield number: " << shield_number);
    }

    savegame->set_reserved_integer(Savegame::LINK_SHIELD, shield_number);

    if (zsdx->game != NULL) {
      zsdx->game->get_link()->initialize_sprites(); // reinitialize Link's sprites
    }
  }
}

// rupees

/**
 * Returns the maximum number of rupees of Link.
 * @return Link's maximum number of rupees (99, 199 or 999)
 */
int Equipment::get_max_rupees(void) {
  return savegame->get_reserved_integer(Savegame::MAX_RUPEES);
}

/**
 * Sets the maximum number of rupees of Link.
 * @param max_rupees Link's maximum number of rupees (99, 199 or 999)
 */
void Equipment::set_max_rupees(int max_rupees) {
  
  if (max_rupees != 99 && max_rupees != 199 && max_rupees != 999) {
    DIE("Illegal maximum number of rupees: " << max_rupees);
  }

  savegame->set_reserved_integer(Savegame::MAX_RUPEES, max_rupees);
}

/**
 * Returns Link's current number of rupees.
 * @return Link's current number of rupees
 */
int Equipment::get_rupees(void) {
  return savegame->get_reserved_integer(Savegame::CURRENT_RUPEES);
}

/**
 * Sets Link's current number of rupees.
 * Exits with an error message if the given number of rupees is not valid.
 * @param rupees Link's new number of rupees
 */
void Equipment::set_rupees(int rupees) {

  if (rupees < 0 || rupees > get_max_rupees()) {
    DIE("Illegal number of rupees: " << rupees);
  }
  
  savegame->set_reserved_integer(Savegame::CURRENT_RUPEES, rupees);
}

/**
 * Adds some rupees to Link.
 * If the maximum number of rupees is achieved, no more rupees are added.
 * @param rupees_to_add number of rupees to add
 */
void Equipment::add_rupees(int rupees_to_add) {

  int max_rupees = get_max_rupees();
  int total = get_rupees() + rupees_to_add;

  set_rupees(MIN(total, max_rupees));
}

/**
 * Removes some rupees from Link.
 * If the number of rupees achieves zero, no more rupees are removed.
 * @param rupees_to_remove number of rupees to remove
 */
void Equipment::remove_rupees(int rupees_to_remove) {

  int total = get_rupees() - rupees_to_remove;
  
  set_rupees(MAX(total, 0));
}

// hearts

/**
 * Returns the maximum number of hearts of Link.
 * @return Link's maximum number of rupees
 */
int Equipment::get_max_hearts(void) {
  return savegame->get_reserved_integer(Savegame::MAX_HEARTS);
}

/**
 * Sets the maximum number of hearts of Link.
 * The program exits with an error message if the given maximum
 * number of hearts is not valid.
 * @param max_hearts Link's maximum number of hearts
 */
void Equipment::set_max_hearts(int max_hearts) {

  if (max_hearts <= 0 || max_hearts > 20) {
    DIE("Illegal maximum number of hearts: " << max_hearts);
  }

  savegame->set_reserved_integer(Savegame::MAX_HEARTS, max_hearts);
}

/**
 * Adds a heart container to Link.
 */
void Equipment::add_heart_container(void) {
  set_max_hearts(get_max_hearts() + 1);
}

/**
 * Returns the current number of hearts of Link.
 * The value returned is actually the number of heart quarters,
 * so a value of 4 means that Link has 1 heart left.
 * @return Link's current number of hearts (in heart quarters)
 */
int Equipment::get_hearts(void) {
  return savegame->get_reserved_integer(Savegame::CURRENT_HEARTS);
}

/**
 * Sets the current number of hearts of Link.
 * The given value is actually the number of heart quarters,
 * so a value of 4 means that Link has 1 heart left.
 * The program exits with an error message if the given number
 * of hearts is not valid.
 * @param hearts Link's new number of hearts (in heart quarters)
 */
void Equipment::set_hearts(int hearts) {

  if (hearts < 0 || hearts > get_max_hearts() * 4) {
    DIE("Illegal number of hearts: " << hearts);
  }

  savegame->set_reserved_integer(Savegame::CURRENT_HEARTS, hearts);
}

/**
 * Gives some hearts to Link.
 * If the maximum number of hearts is achieved, no more hearts are added.
 * @param hearts_to_add number of hearts to add (in heart quarters)
 */
void Equipment::add_hearts(int hearts_to_add) {

  int max_hearts = get_max_hearts() * 4;
  int total = get_hearts() + hearts_to_add;

  set_hearts(MIN(total, max_hearts));
}

/**
 * Removes some hearts from Link.
 * If the number of hearts achieves zero, the game over sequence is started.
 * @param hearts_to_remove number of hearts to remove
 */
void Equipment::remove_hearts(int hearts_to_remove) {

  int total = get_hearts() - hearts_to_remove;
  
  set_hearts(MAX(total, 0));
}

/**
 * Restores all the hearts.
 */
void Equipment::restore_all_hearts(void) {
  set_hearts(get_max_hearts());
}

/**
 * Returns whether the player is running out of hearts.
 * The function returns true if the number of hearts is lower
 * than or equal to 25% of the maximum.
 * @return true if the player is running out of hearts
 */
bool Equipment::needs_hearts(void) {
  return get_hearts() <= get_max_hearts();
}

/**
 * Returns the current number of pieces of heart of Link.
 * @returns Link's current number of pieces of heart, between 0 and 3
 */
int Equipment::get_nb_pieces_of_heart(void) {
  return savegame->get_reserved_integer(Savegame::PIECES_OF_HEART);
}

/**
 * Returns whether Link has the specified piece of heart.
 * @param piece_of_heart_id index of the piece of heart, between 0
 * and the number of pieces of hearts in the game (which cannot
 * be greater than 80)
 */
bool Equipment::has_piece_of_heart(int piece_of_heart_id) {

  int index = Savegame::FIRST_PIECE_OF_HEART + piece_of_heart_id;
  
  if (index < Savegame::FIRST_PIECE_OF_HEART || index > Savegame::LAST_PIECE_OF_HEART) {
    DIE("Illegal piece of heart index: " << piece_of_heart_id);
  }

  return savegame->get_reserved_integer(index) != 0;
}

/**
 * Adds a piece of heart to Link.
 * @param piece_of_heart_id index of the piece of heart, between 1
 * and the number of pieces of hearts in the game - 1
 */
void Equipment::add_piece_of_heart(int piece_of_heart_id) {

  // note: the piece_of_heart sound is played by the collectable when it is picked

  if (has_piece_of_heart(piece_of_heart_id)) {
    DIE("The player already has piece of heart #" << piece_of_heart_id);
  }

  savegame->set_reserved_integer(Savegame::FIRST_PIECE_OF_HEART + piece_of_heart_id, 1);

  // check whether Link has a new heart

  int nb_pieces_of_heart = get_nb_pieces_of_heart() + 1;
  if (nb_pieces_of_heart < 4) {
    // no new heart
    savegame->set_reserved_integer(Savegame::PIECES_OF_HEART, nb_pieces_of_heart);
  }
  else {
    // new heart container
    savegame->set_reserved_integer(Savegame::PIECES_OF_HEART, 0);
    add_heart_container();
  }

  restore_all_hearts();
}

// magic

/**
 * Returns the maximum level of Link's magic bar.
 * @return the maximum level of magic (0, 42 or 84 points)
 */
int Equipment::get_max_magic(void) {
  return savegame->get_reserved_integer(Savegame::MAX_MAGIC);
}

/**
 * Sets the maximum level of Link's magic bar.
 * Exits with an error message if the value specified
 * if not valid.
 * @param max_magic the maximum level of magic (0, 42 or 84 points)
 */
void Equipment::set_max_magic(int max_magic) {
  
  if (max_magic != 0 && max_magic != 42 && max_magic != 84) {
    DIE("Illegal maximum number of magic points: " << max_magic);
  }

  savegame->set_reserved_integer(Savegame::MAX_MAGIC, max_magic);

  restore_all_magic();
}

/**
 * Returns the current number of magic points of Link.
 * @return Link's current number of magic points (0 to 28)
 */
int Equipment::get_magic(void) {
  return savegame->get_reserved_integer(Savegame::CURRENT_MAGIC);
}

/**
 * Sets the current number of magic points of Link.
 * The program exits with an error message if the given value
 * is not valid.
 * @param magic Link's new number of magic points (0 to 84)
 */
void Equipment::set_magic(int magic) {

  if (magic < 0 || magic > get_max_magic()) {
    DIE("Illegal number of magic points: " << magic);
  }

  savegame->set_reserved_integer(Savegame::CURRENT_MAGIC, magic);
}

/**
 * Adds some magic points to Link.
 * If the maximum value is achieved, no more magic points are added.
 * @param magic_to_add number of magic points to add
 */
void Equipment::add_magic(int magic_to_add) {

  int max_magic = get_max_magic();
  int total = get_magic() + magic_to_add;

  set_magic(MIN(total, max_magic));
}

/**
 * Removes some magic points from Link.
 * If the number of magic points achieves zero, no more magic points
 * are removed.
 * @param magic_to_remove number of magic poits to remove
 */
void Equipment::remove_magic(int magic_to_remove) {

  int total = get_magic() - magic_to_remove;
  
  set_magic(MAX(total, 0));
}

/**
 * Restores all magic points.
 */
void Equipment::restore_all_magic(void) {
  set_magic(get_max_magic());
}

/**
 * Returns whether the player needs magic.
 * The function returns true if the player has a magic bar
 * and has 0 magic points left.
 * @return true if the player needs magic
 */
bool Equipment::needs_magic(void) {
  return get_magic() == 0 && get_max_magic() != 0;
}

/**
 * Returns whether the magic bar is decreasing continuously.
 * @return true if the magic bar is decreasing, false otherwise
 */
bool Equipment::is_magic_decreasing(void) {
  return this->magic_decrease_delay != 0;
}

/**
 * Starts removing magic continuously.
 * @param delay delay in miliseconds between two decreases
 */
void Equipment::start_removing_magic(Uint32 delay) {

  if (delay <= 0) {
    DIE("Illegal magic bar decrease delay: " << delay);
  }

  if (get_magic() > 0) {
    this->magic_decrease_delay = delay;
    this->next_magic_decrease_date = SDL_GetTicks();
    
    // the magic points will be removed by the update() function
  }
}

/**
 * Stops removing magic continuously.
 */
void Equipment::stop_removing_magic(void) {
  this->magic_decrease_delay = 0;
}

// bombs

/**
 * Returns the maximum number of bombs of Link.
 * @return Link's maximum number of bombs (0, 10, 30 or 99)
 */
int Equipment::get_max_bombs(void) {
  return savegame->get_reserved_integer(Savegame::MAX_BOMBS);
}

/**
 * Sets the maximum number of bombs of Link.
 * @param max_bombs Link's maximum number of bombs (0, 10, 30 or 99)
 */
void Equipment::set_max_bombs(int max_bombs) {
  
  if (max_bombs != 0 && max_bombs != 10 && max_bombs != 30 && max_bombs != 99) {
    DIE("Illegal maximum number of bombs: " << max_bombs);
  }

  savegame->set_reserved_integer(Savegame::MAX_BOMBS, max_bombs);
}

/**
 * Returns the current number of bombs.
 * @return Link's current number of bombs
 */
int Equipment::get_bombs(void) {
  return savegame->get_reserved_integer(Savegame::CURRENT_BOMBS);
}

/**
 * Sets the current number of bombs of Link.
 * The program exits with an error message if the given number
 * of bombs is not valid.
 * @param bombs Link's new number of bombs
 */
void Equipment::set_bombs(int bombs) {

  if (bombs < 0 || bombs > get_max_bombs()) {
    DIE("Illegal number of bombs: " << bombs);
  }

  savegame->set_reserved_integer(Savegame::CURRENT_BOMBS, bombs);
}

/**
 * Adds some bombs to Link.
 * If the maximum number of bombs is achieved, no more bombs are added.
 * @param bombs_to_add number of bombs to add
 */
void Equipment::add_bombs(int bombs_to_add) {

  int max_bombs = get_max_bombs();
  int total = get_bombs() + bombs_to_add;

  set_bombs(MIN(total, max_bombs));
}

/**
 * Removes a bomb from Link.
 * If the number of bombs is already zero, nothing happens.
 */
void Equipment::remove_bomb(void) {

  int total = get_bombs() - 1;
  
  set_bombs(MAX(total, 0));
}

/**
 * Returns whether the player needs bombs.
 * The function returns true if the player has obtained the bombs
 * but has 0 bombs left.
 * @return true if the player needs bombs
 */
bool Equipment::needs_bombs(void) {
  return has_inventory_item(InventoryItem::BOMBS)
    && get_bombs() == 0;
}

/**
 * Returns the maximum number of arrows of Link.
 * @return Link's maximum number of arrows (0, 10, 30 or 99)
 */
int Equipment::get_max_arrows(void) {
  return savegame->get_reserved_integer(Savegame::MAX_ARROWS);
}

/**
 * Sets the maximum number of arrows of Link.
 * @param max_arrows Link's maximum number of arrows (0, 10, 30 or 99)
 */
void Equipment::set_max_arrows(int max_arrows) {
  
  if (max_arrows != 0 && max_arrows != 10 && max_arrows != 30 && max_arrows != 99) {
    DIE("Illegal maximum number of arrows: " << max_arrows);
  }

  savegame->set_reserved_integer(Savegame::MAX_ARROWS, max_arrows);
}

/**
 * Returns the current number of arrows.
 * @return Link's current number of arrows
 */
int Equipment::get_arrows(void) {
  return savegame->get_reserved_integer(Savegame::CURRENT_ARROWS);
}

/**
 * Sets the current number of arrows of Link.
 * The program exits with an error message if the given number
 * of arrows is not valid.
 * @param arrows Link's new number of arrows
 */
void Equipment::set_arrows(int arrows) {

  if (arrows < 0 || arrows > get_max_arrows()) {
    DIE("Illegal number of arrows: " << arrows);
  }

  savegame->set_reserved_integer(Savegame::CURRENT_ARROWS, arrows);

  if (has_inventory_item(InventoryItem::BOW)) {

    // the bow item changes depending on whether the player has arrows
    if (arrows == 0) {
      give_inventory_item(InventoryItem::BOW, 1); // bow without arrows
    }
    else {
      give_inventory_item(InventoryItem::BOW, 2); // bow with arrows
    }
  }
}

/**
 * Adds some arrows to Link.
 * If the maximum number of arrows is achieved, no more arrows are added.
 * @param arrows_to_add number of arrows to add
 */
void Equipment::add_arrows(int arrows_to_add) {

  int max_arrows = get_max_arrows();
  int total = get_arrows() + arrows_to_add;

  set_arrows(MIN(total, max_arrows));
}

/**
 * Removes an arrow from Link.
 * If the number of arrows is already zero, nothing happens.
 */
void Equipment::remove_arrow(void) {

  int total = get_arrows() - 1;
  
  set_arrows(MAX(total, 0));
}

/**
 * Returns whether the player needs arrows.
 * The function returns true if the player has obtained the bow
 * but has 0 arrows left.
 * @return true if the player needs arrows
 */
bool Equipment::needs_arrows(void) {
  return has_inventory_item(InventoryItem::BOW)
    && get_arrows() == 0;
}

/**
 * Returns whether the player has obtained the specified item.
 * For most of the items, the value returned is always 0 or 1.
 * Some items have several variants (for example a bottle):
 * for such items, the value returned indicates the variant
 * and may be greater than 1.
 * @param item_id id of the item
 * @return a value indicating the possession state of this item.
 */
int Equipment::has_inventory_item(InventoryItem::ItemId item_id) {

  int index = Savegame::FIRST_INVENTORY_ITEM + item_id;
  return savegame->get_reserved_integer(index);
}

/**
 * Gives an item of the inventory to the player.
 * This is equivalent to give_inventory_item(item_id, 1).
 * @param item_id the item to give
 */
void Equipment::give_inventory_item(InventoryItem::ItemId item_id) {
  give_inventory_item(item_id, 1);
}

/**
 * Sets the possession state of an item.
 * @param item_id the item to set
 * @param variant the variant of the item to give to the player,
 * or zero to remove the item
 */
void Equipment::give_inventory_item(InventoryItem::ItemId item_id, int variant) {

  int index = Savegame::FIRST_INVENTORY_ITEM + item_id;
  savegame->set_reserved_integer(index, variant);
}

/**
 * Removes from the player an item of the inventory.
 * This is equivalent to give_inventory_item(item_id, 0).
 * @param item_id the item to remove
 */
void Equipment::remove_inventory_item(InventoryItem::ItemId item_id) {
  give_inventory_item(item_id, 0);
}

/**
 * Returns the item currently assigned to a slot.
 * @param slot slot of the item to get (0 for X or 1 for V)
 */
InventoryItem::ItemId Equipment::get_item_assigned(int slot) {
  int index = Savegame::ITEM_SLOT_0 + slot;
  return (InventoryItem::ItemId) savegame->get_reserved_integer(index);
}

/**
 * Assigns an item to a slot.
 * The program exits with an error message if the specified item
 * cannot be equiped or if the player does not have it
 * @param slot slot to set (0 for X or 1 for V)
 * @param item_id the item to assign to this slot
 */
void Equipment::set_item_assigned(int slot, InventoryItem::ItemId item_id) {

  if (!has_inventory_item(item_id)) {
    DIE("Cannot assign item " << item_id << " because the player does not have it");
  }

  InventoryItem *item = InventoryItem::get_item(item_id);
  if (!item->is_attributable()) {
    DIE("Cannot assign item " << item_id << " because it is not attributable");
  }

  int index = Savegame::ITEM_SLOT_0 + slot;
  savegame->set_reserved_integer(index, item_id);
}
