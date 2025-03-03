



#ifdef MAINBOARD
void cmd_mainboard_weichen(int argc, char **argv)
{
#ifdef ESP32
	Serial.printf("%s CALLED with VAL: %s %s!\n",__func__, argv[1],argv[2]);
	weiche_set( weichen, atoi(argv[1]), atoi(argv[2]) );	
#else
	printf("%s CALLED with VAL: %s %s!\n",__func__, argv[1],argv[2]);

#endif
}
void cmd_mainboard_led_set(int argc, char **argv)
{
#ifdef ESP32
	Serial.printf("%s CALLED with VAL: %s %s!\n",__func__, argv[1],argv[2]);
	led_set( led, atoi(argv[1]), atoi(argv[2]));
//	weiche_set( weichen, atoi(argv[1]), atoi(argv[2]) );	
#else
	printf("%s CALLED with VAL: %s %s!\n",__func__, argv[1],argv[2]);
#endif
}
void cmd_mainboard_led_write(int argc, char **argv)
{
#ifdef ESP32
	Serial.printf("%s CALLED with VAL: %s!\n",__func__, argv[1]);

	led_write( led ); 
//	weiche_set( weichen, atoi(argv[1]), atoi(argv[2]) );	
#else
	printf("%s CALLED with VAL: %s %s!\n",__func__, argv[1],argv[2]);
#endif
}

#endif // MAINBOARD
