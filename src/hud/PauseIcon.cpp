/*
 * Copyright (C) 2009 Christopho, Zelda Solarus - http://www.zelda-solarus.com
 * 
 * Zelda: Mystery of Solarus DX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Zelda: Mystery of Solarus DX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "hud/PauseIcon.h"
#include "KeysEffect.h"
#include "ResourceManager.h"
#include "Sprite.h"

/**
 * Constructor.
 * @param keys_effect current effect of the main keys
 * (here we are interested in the pause key)
 * @param x x coordinate of the top-left corner of the icon on the destination surface
 * @param y y coordinate of the top-left corner of the icon on the destination surface
 */
PauseIcon::PauseIcon(KeysEffect *keys_effect, int x, int y):
  HudElement(x, y, 72, 24), keys_effect(keys_effect) {

  // static icon
  this->pause_key_effect_displayed = keys_effect->get_pause_key_effect();
  this->img_pause_icon = ResourceManager::load_image("hud/pause_icon.png");

  // flipping icon
  this->is_flipping = false;
  this->sprite_pause_icon = new Sprite("hud/pause_icon_flip");

  rebuild();
}

/**
 * Destructor.
 */
PauseIcon::~PauseIcon(void) {
  delete sprite_pause_icon;
  SDL_FreeSurface(img_pause_icon);
}

/**
 * This function is called repeatedly, it checks whether
 * the icon should change.
 */
void PauseIcon::update(void) {

  bool need_rebuild = false;

  // text shown
  if (!is_flipping) {

    KeysEffect::PauseKeyEffect pause_key_effect = keys_effect->get_pause_key_effect();

    if (pause_key_effect_displayed != pause_key_effect) {

      pause_key_effect_displayed = pause_key_effect;

      sprite_pause_icon->start_animation();
      is_flipping = true;
      need_rebuild = true;
    }
  }
  else {
    sprite_pause_icon->update();
    need_rebuild = true;

    if (sprite_pause_icon->is_animation_finished()) {
      is_flipping = false;
    }
  }

  // icon opacity
  if (keys_effect->is_pause_key_enabled() && get_opacity() == 128) {
    set_opacity(255);
  }
  else if (!keys_effect->is_pause_key_enabled() && get_opacity() == 255) {
    set_opacity(128);
  }

  // redraw the surface if something has changed
  if (need_rebuild) {
    rebuild();
  }
}

/**
 * Redraws the icon on the surface.
 */
void PauseIcon::rebuild(void) {

  HudElement::rebuild();
  
  if (!is_flipping) {

    // draw the static image of the icon, with the current
    // pause key effect

    SDL_Rect icon_position = {0, 0, 72, 24};
    
    // draw the pause icon with some text (Save, Return...)
    icon_position.y = 24 * pause_key_effect_displayed;

    SDL_BlitSurface(img_pause_icon, &icon_position, surface_drawn, NULL);
  }
  else {
    // draw the flipping sprite
    sprite_pause_icon->display(surface_drawn, 24, 0);
  }
}
