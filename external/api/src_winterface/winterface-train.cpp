
#ifdef TRAIN
void cmd_set_target_speed(int argc, char**argv)
{
//	tc_speedset( atoi( argv[1] ));
}; 

void cmd_set_dir( int argc, char **argv)
{
#ifdef ESP32
//	Serial.printf("%s CALLED! with argument: %d\n",__func__, atoi(argv[1]));
	dr_set(dr,atoi( argv[1] ) );
#else
	printf("%s CALLED! with argument: %d\n",__func__, atoi(argv[1]));
#endif


}
void cmd_tc_ramp(int argc, char **argv)
{
#ifdef ESP32
//	Serial.printf("%s CALLED!\n",__func__);
	tc_ramp(tcp, atoi( argv[1] ) , atoi( argv[2] ) );
#else
	printf("%s CALLED!\n",__func__);
#endif
}
void cmd_tc_speed(int argc, char **argv)
{
//	printf("%d \n", tcp->target_speed );
#ifdef ESP32
//	Serial.printf("%s CALLED with VAL: %s !\n",__func__, argv[1]);
	tc_settarget(tcp, atoi( argv[1] ) );
//	Serial.printf("Target Speed: %d \n", tcp->target_speed );
#else
	printf("%s CALLED with VAL: %s !\n",__func__, argv[1]);
#endif
}

void cmd_tc_report(int argc, char** argv)
{
	return;
}
void cmd_tc_stop(int argc, char **argv)
{
#ifdef ESP32
//	Serial.printf("%s CALLED!\n",__func__);
	tc_stop(tcp );
#else
	printf("%s CALLED!\n",__func__);
#endif
}

#endif // ifdef TRAIN
