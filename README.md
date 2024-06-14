# Brainfuck++ Documentation

## Introduction
Brainfuck++ is an esoteric programming language inspired by Brainfuck but with enhancements and extensions to facilitate writing and understanding of programs.

## Installation Guide
- Download Windows [Release](https://github.com/lizardwine/brainfuckpp/raw/master/release/win-release.zip)
- Download Linux [Release](https://github.com/lizardwine/brainfuckpp/raw/master/release/linux-release.zip)

## Key Features
- **Expanded Instructions:** Introduces new instructions and functionalities to increase expressiveness and programming capability.
- **Improved Readability:** Changes have been made to make programs written in Brainfuck++ more readable and easier to understand.
- **Functions:** The ability to define functions and call them from within the program.
- **Brainfuck Compatibility:** While Brainfuck++ is an extension of Brainfuck, programs written in Brainfuck are compatible and can be executed in the Brainfuck++ interpreter.

## Instructions
### Standard Brainfuck Instructions
Description of standard Brainfuck instructions such as `>`, `<`, `+`, `-`, `[`, `]`, `.`, `,`
- `>` Move the pointer to the right
- `<` Move the pointer to the left
- `+` Increase the value at the pointer
- `-` Decrease the value at the pointer
- `[` Starts a loop if the value at the pointer is non-zero
- `]` Ends a loop if the value at the pointer is zero
- `.` Print the value at the pointer as a character
- `,` Read a value and store it in the pointer

### New Instructions in Brainfuck++
Description of the new instructions in Brainfuck++
- `:` Print the value at the pointer as a number.
- `;` Read a number and store it in the pointer.
- `{` Start the definition of a function.
- `}` End the definition of a function.
- `(` Starts importing a file.
- `)` Ends importing a file.
- `@` Toggle the use of local memory or global memory.
- `$` Copy the value at the pointer of the foreign memory in the current memory.
- `!` Call a function with the current value of the pointer as the ID(ID's start at 1).
- `&` Store the current memory position in the pointer.
- `=` It goes to the memory location using the pointer value as the target memory address.
- `%` Returns the pointer value in a function or terminates the program if not within a function.
- `#` Ignore the following instruction until a matching `\n` is found.

## Example Program
`lib.bf`
```bf
{  
    # Sum function
    @ # change to global memory for copy arguments
    <
    @ # change to local memory
    $ # copy the value
    @ 
    <
    @
    >$
    # Sum logic
    >[-]
    <<[>+>+<<-]
    >>[<<+>>-]
    <
    % # Return the result
}
```

`main.bf`
```bf
(lib.bf) # Imports the file with sum function.
>++      # Set first parameter to 2
>+++     # Set second parameter to 3
>+       # Set function ID to 1
!        # Call the sum function
:        # Print the result, which is 5
```

## Additional Examples
### Using Standard Library
```bf
(std.bf)
{ # ID 1
    # Print "TRUE"
    +++++++[>++++++++++++<-]>.<+[>-<-]>-.<+[>++<-]>+.<++++[>----<-]>.<
}
{ # ID 2
    # Print "FALSE"
    +++++++[>++++++++++<-]>.<++[>--<-]>-.<++[>+++++<-]>+.<++[>+++<-]>+.<++[>-------<-]>.<
}
+ # if 1
>+ # is equals
>+ # to 1
>+ # then call function with ID 1
>++ # else call function with ID 2
>>+++ +++[<+++ +++>-]< # set function ID to 36 (standard library condtional function)
! # call it
```

## Standard Library Reference

### What does mean memory(x) ?
>`x` is the relative memory position. `x` must be added to the current memory position to get the target memory position.

>Example: 
>
> If the current memory position is 10, and `x` is 2, then the target memory position is 12.

### Functions
Standard Library Functions ID's starts at 33 and ends at 64.

Here is the list of current standard library functions:
- `ID 33` copy memory memory(-1) value to memory(0)
- `ID 34` multiply memory(-2) and memory(-1) and store in memory(0)
- `ID 35` print new line
- `ID 36` conditional function

    if memory(-5) is memory(-4) than memory(-3) then call memory(-2) else call memory(-1)

   #### Codes List

   the mode of the conditional function, set it in memory(-4)

   - Equals: 1

   - Greater: 2

   - Greater or equals: 3
