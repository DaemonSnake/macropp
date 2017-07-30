# macro++, a Unix/GNU command-line macro-system
When feeding macro++ some text it will look for the following motif :</br>
```c
[@COMMAND_NAME ARG_0 @, ARG_1 @, ... @]
```
The grammar is the following:
```ebnf
<COMMAND> ::= {"[@" COMMAND_NAME {ANYTHING {"@," ANYTHING}* }? "@]" }
<COMMAND_NAME> ::= {"" | "BALANCED" | "BALANCED_SW" | "LOOK" | "LOOK_SW" | "MACRO" | "MACRO_OP" | "COUNTER" | "FORMAT" | "STRLEN" | "SYSTEM" | "SWITCH" }
```
<p>This motif is called a command.</br>
Macro++ will not output the motif and instead generate some text at the
place the command requires it.</p>
<p>Commands can be chained and won't match each other, they also can be
nested meaning a command argument can be an other command. In such cases
the root command will not evaluate the command argument and will simply proccess
it as pure text.</p>
<p>Even more a command(s) can be used to produce new commands.</br>
</p>
example:</br>

```c
[@FORMAT "%OPEN" @]COMMAND_NAME ARG_0 [@FORMAT "%CLOSE" @]
```
or
```c
[@FORMAT "%OPEN% %%CLOSE" @, COMMAND_NAME @, ARG_0 @]
```
will have the same effect as
```c
[@COMMAND_NAME @, ARG_0 @]
```
</br>
<h4>Here goes the list of the commands :</h4>

<h3>Description of each command</h3>
- BALANCED(_SW)
Description:
SW stands for swallow and removes the IN and OUT character!<br>
Syntax:

```ebnf
[@BALANCED|BALANCED_SW 'IN' @, 'OUT' @, AFTER (@, BEFORE) @]
```
<h6>Example</h6>
input file:

```c
[@BALANCED '{' @, '}' @, printf("after block\n"); @, printf("before block\n"); @]
{
}
```
output:

```c
printf("before block\n");
{
}printf("after block\n");
```
- LOOK(_SW)
Description:
SW stands for swallow and removes the IN and OUT character!
Syntax:

```ebnf
[@LOOK|LOOK_SW WHAT @, AFTER (@, BEFORE) @]
```
<h6>Example</h6>
input file:

```c
[@LOOK TEST @, BEFORE_ @, _AFTER @] ... TEST ...
```
output:

```c
... BEFORE_TEST_AFTER ...
```

- MACRO(_OP)
- FORMAT
Description:
Syntax:

```ebnf
[@FORMAT STRING_LITERAL (@, ARGUMENTS)* @]
```
<h6>Example</h6>
input file:

```c
[@FORMAT "A%Z" @, BCDEFGHIJKLMNOPQRSTUVWXY @]
```
output:

```c
ABCDEFGHIJKLMNOPQRSTUVWXYZ
```
- STRLEN

Description:
find the length of a string literal and replace the command with said length in text
Syntax:

```ebnf
[@STRLEN STRING_LITERAL @]
```
<h6>Example</h6>
input file:

```c
[@STRLEN "Hello world!" @]
```
output:

```c
12
```

The orignal goal was to make to tool that corrects the short-commings of
the C's proprocessor but it can be used in many other ways.
