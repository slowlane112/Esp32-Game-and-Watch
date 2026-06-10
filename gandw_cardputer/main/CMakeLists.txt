set(model_assets)

if(MODEL STREQUAL "cardputer_multi_screen")

	list(APPEND model_assets
		"menu_cardputer_multi_screen.raw"
		"Game & Watch Black Jack.gw"
		"Game & Watch Bomb Sweeper.gw"
		"Game & Watch Donkey Kong II.gw"
		"Game & Watch Donkey Kong.gw"
		"Game & Watch Gold Cliff.gw"
		"Game & Watch Green House.gw"
		"Game & Watch Mickey & Donald.gw"
		"Game & Watch Oil Panic.gw"
		"Game & Watch Pinball.gw"
		"Game & Watch Safe Buster.gw"
		"Game & Watch Squish.gw"
		"Game & Watch Zelda.gw"
	)

else()

	list(APPEND model_assets
		"menu_cardputer.raw"
		"Game & Watch Ball.gw"
		"Game & Watch Chef.gw"
		"Game & Watch Egg.gw"
		"Game & Watch Fire Attack.gw"
		"Game & Watch Fire (Silver).gw"
		"Game & Watch Fire (Wide Screen).gw"
		"Game & Watch Flagman.gw"
		"Game & Watch Helmet (CN-17 version).gw"
		"Game & Watch Judge (green version).gw"
		"Game & Watch Lion.gw"
		"Game & Watch Manhole (Gold).gw"
		"Game & Watch Manhole (New Wide Screen).gw"
		"Game & Watch Mario's Cement Factory (New Wide Screen).gw"
		"Game & Watch Mario The Juggler.gw"
		"Game & Watch Mickey Mouse (Wide Screen).gw"
		"Game & Watch Octopus.gw"
		"Game & Watch Parachute.gw"
		"Game & Watch Popeye (Wide Screen).gw"
		"Game & Watch Snoopy Tennis.gw"
		"Game & Watch Tropical Fish.gw"
		"Game & Watch Turtle Bridge.gw"
		"Game & Watch Vermin.gw"
		"Game & Watch Donkey Kong Jr. (New Wide Screen).gw"
		"Game & Watch Balloon Fight (New Wide Screen).gw"
		"Game & Watch Climber (New Wide Screen).gw"
		"Game & Watch Super Mario Bros. (New Wide Screen).gw"
	)

endif()

idf_component_register(
	SRC_DIRS "."
	INCLUDE_DIRS "."
	EMBED_FILES "${model_assets}"
)
