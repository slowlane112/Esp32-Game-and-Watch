set(model_assets)

if(MODEL STREQUAL "single_screen_2_button")

	list(APPEND model_assets
		"menu_single_screen_2_button.raw"
		"Game & Watch Ball.gw"
		"Game & Watch Vermin.gw"
		"Game & Watch Fire (Silver).gw"
		"Game & Watch Helmet (CN-17 version).gw"
		"Game & Watch Parachute.gw"
		"Game & Watch Octopus.gw"
		"Game & Watch Popeye (Wide Screen).gw"
		"Game & Watch Chef.gw"
		"Game & Watch Fire (Wide Screen).gw"
		"Game & Watch Turtle Bridge.gw"
		"Game & Watch Tropical Fish.gw"
		"Game & Watch Mario The Juggler.gw"	
	)

elseif(MODEL STREQUAL "multi_screen_2_button")

	list(APPEND model_assets
		"menu_multi_screen_2_button.raw"
		"Game & Watch Oil Panic.gw"
		"Game & Watch Pinball.gw"
		"Game & Watch Safe Buster.gw"
	)

elseif(MODEL STREQUAL "multi_screen_dpad")

	list(APPEND model_assets
		"menu_multi_screen_dpad.raw"
		"Game & Watch Donkey Kong.gw"
		"Game & Watch Green House.gw"
		"Game & Watch Donkey Kong II.gw"
		"Game & Watch Bomb Sweeper.gw"
		"Game & Watch Gold Cliff.gw"
		"Game & Watch Zelda.gw"
	)

else()

	list(APPEND model_assets
		"menu_single_screen_4_button.raw"
		"Game & Watch Ball.gw"
		"Game & Watch Flagman.gw"
		"Game & Watch Vermin.gw"
		"Game & Watch Fire (Silver).gw"
		"Game & Watch Judge (green version).gw"
		"Game & Watch Manhole (Gold).gw"
		"Game & Watch Helmet (CN-17 version).gw"
		"Game & Watch Lion.gw"
		"Game & Watch Parachute.gw"
		"Game & Watch Octopus.gw"
		"Game & Watch Popeye (Wide Screen).gw"
		"Game & Watch Chef.gw"
		"Game & Watch Mickey Mouse (Wide Screen).gw"
		"Game & Watch Egg.gw"
		"Game & Watch Fire (Wide Screen).gw"
		"Game & Watch Turtle Bridge.gw"
		"Game & Watch Fire Attack.gw"
		"Game & Watch Snoopy Tennis.gw"
		"Game & Watch Mario's Cement Factory (New Wide Screen).gw"
		"Game & Watch Manhole (New Wide Screen).gw"
		"Game & Watch Tropical Fish.gw"
		"Game & Watch Mario The Juggler.gw"
	)

endif()

idf_component_register(
	SRC_DIRS "."
	INCLUDE_DIRS "."
	EMBED_FILES "${model_assets}"
)
