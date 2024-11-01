SimpleCLI cli;

Command cmdPing;
Command cmdSet;     // set {temp/speed} [value]
Command cmdPrint;   // print {temp/speed/all}
Command cmdPreset;  // preset [n] [value]
Command cmdDebug;   // debug [set/show/reset] [temp/speed] {on/off}

//_debugTemp
void settupCli() {
  cmdPing = cli.addCmd("ping", pingCallback);
  cmdPing.addPositionalArgument("n", "1");
  cmdPing.setDescription(" Responds with a ping n-times");

  cmdSet = cli.addCmd("set", setCallback);
  cmdSet.addPositionalArgument("a");
  cmdSet.addPositionalArgument("value");
  cmdSet.setDescription("usage: set {temp/speed} [value]");

  cmdPrint = cli.addCmd("print", printCallback);
  cmdPrint.addPositionalArgument("a", "all");
  cmdPrint.setDescription("usage: print {temp/speed/all}");

  cmdPreset = cli.addCmd("preset", presetCallback);
  cmdPreset.addPositionalArgument("a");
  cmdPreset.addPositionalArgument("value");
  cmdPreset.setDescription("usage: preset [a] [value]");

  cmdPreset = cli.addCmd("debug", debugCallback);
  cmdPreset.addPositionalArgument("a");
  cmdPreset.addPositionalArgument("b");
  cmdPreset.addPositionalArgument("c");
  cmdPreset.setDescription("usage: debug [set/show/reset] [temp/speed] {on/off}");


  cli.setOnError(errorCallback);
}
void setCallback(cmd* c) {
  Command cmd(c);  // Create wrapper object

  Argument _selectArg = cmd.getArgument("a");
  Argument _valueArg = cmd.getArgument("value");

  String selectArg = _selectArg.getValue();
  int16_t value = _valueArg.getValue().toInt();

  if (selectArg == "temp") {
    setTemp(value);
  } else if (selectArg == "speed") {
    setStepperSpeed(value);
  } else {
    print("Unknow value, valid is temp/speed\n");
  }
}
void printCallback(cmd* c) {
  Command cmd(c);  // Create wrapper object
  Argument _selectArg = cmd.getArgument("a");

  String selectArg = _selectArg.getValue();

  if (selectArg == "temp") {
    print("temp: ");
    println((String)SET_TEMP);
  } else if (selectArg == "speed") {
    print("speed: ");
    println((String)SET_SPEED);
  } else if (selectArg == "all") {
    print("\ttemp: ");
    println((String)SET_TEMP);
    print("\tspeed: ");
    println((String)SET_TEMP);

  } else {
    print("Unknow value, valid is temp/speed/all\n");
  }
}

void presetCallback(cmd* c) {
  //Command cmd(c);  // Create wrapper object
  //Argument _valueArg = cmd.getArgument("value");
  //Argument _selectArg = cmd.getArgument("a");

  //String selectArg = _selectArg.getValue();
  //int16_t value = _valueArg.getValue().toInt();
}
void debugCallback(cmd* c) {  //usage: debug [set/show/reset] [temp/speed] {true/false}
  Command cmd(c);             // Create wrapper object
  Argument _select1Arg = cmd.getArgument("a");
  String select1Arg = _select1Arg.getValue();

  if (select1Arg == "set") {
    Argument _select2Arg = cmd.getArgument("b");
    String select2Arg = _select2Arg.getValue();
    Argument _select3Arg = cmd.getArgument("c");
    String select3Arg = _select3Arg.getValue();
    if (select2Arg == "temp") {
      if (select2Arg == "true" || select2Arg == "false") {
        _debugTemp = select3Arg;
      } else{
        print("use true/false\n");
      }
    }
  } else if (select1Arg == "show") {
  } else if (select1Arg == "reset") {
  } else {
    print("Unknow value, valid is load/store/print/dump\n");
  }
}
void cliTick() {
  if (Serial.available()) {
    // Read out string from the serial monitor
    String input = Serial.readStringUntil('\n');

    // Echo the user input
    Serial.print("# ");
    Serial.println(input);

    // Parse the user input into the CLI
    cli.parse(input);
  }
}



void pingCallback(cmd* c) {
  Command cmd(c);  // Create wrapper object

  Argument numberArg = cmd.getArgument("n");
  uint8_t n = numberArg.getValue().toInt();
  for (int i = 0; i < n; i++) Serial.println("pong");
}

void errorCallback(cmd_error* e) {
  CommandError cmdError(e);  // Create wrapper object

  Serial.print("ERROR: ");
  Serial.println(cmdError.toString());

  if (cmdError.hasCommand()) {
    Serial.print("Did you mean \"");
    Serial.print(cmdError.getCommand().toString());
    Serial.println("\"?");
  }
}
void print(String s) {
  Serial.print(s);
}
void println(String s) {
  Serial.println(s);
}