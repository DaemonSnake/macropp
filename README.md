# postfix
Postfix is unix command-line text-generator/macro-system.
When feeding postfix some text it will look for the following motif :
     "[@COMMAND_NAME ARG_0 @, ARG_1 @, ... @]

This motif is called a command.
Postfix will not output the motif and instead generate some text at the
place the command requires it.
Commands can be chained and won't match each other, they also can be
nested meaning a command argument can be an other command. In such cases
the root command will not evaluate the command argument and will simply proccess
it as pure text.
Even more a command(s) can be used to produce new commands.
example:
    [@FORMAT %OPEN @]COMMAND_NAME [@FORMAT %SEP @] ARG_0 [@FORMAT %CLOSE @]
    will have the same effect as [@COMMAND_NAME @, ARG_0 @]

Here goes the list of the commands :
-BALENCED/BALENCED_SW
-LOOK/LOOK_SW
-MACRO/MACRO_EVAL
-COUNTER
-FORMAT
-STRLEN

The orignal goal was to make to tool that corrects the short-commings of
the C's proprocessor but it can be used in many other ways.
