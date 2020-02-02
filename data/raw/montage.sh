#!/bin/bash

montage empty_card.png \
        false_alarm.png \
        false_repair_1.png \
        false_repair_2.png \
        place_bomb_1.png \
        place_bomb_2.png \
        place_bomb_3.png \
        set_up_jammer.png \
        repair.png \
        block.png \
        demine.png \
        hide.png \
        reinforce_2.png \
        release.png \
        reinforce_1.png \
        track.png \
        -background none -tile 4x4 -geometry 400x500+0  png32:cards.png
