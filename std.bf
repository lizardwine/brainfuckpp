{S # 33
   # Copy values from memory(-1) to memory(0)
    @
    <
    @
    $
    @
    >
    $
}
{S # 34
    # Multiply values memory(-2) * memory(-1) and store in memory(0)
    @<<@
    >>$
    @>@
    >$
    <<<
    [-]
    >[-]
    >[<+>-]
    <[
        >>[<+<<+>>>-]<<<[>>>+<<<-]
        >-
    ]
    >%
}
{S # 35 
    # Print new line
    ++[>+++++<-]>.<
}
{S # 36
    # Conditional function
    # if memory(-5) is memory(-4)(check codes list) than memory(-3) then call memory(-2) else call memory(-1)
    # codes list
    # 1 - equals
    # 2 - greater
    # 3 - greater or equals
    
    @ # global
    <<<<<
    @ # local
    $ # memory(0)
    >
    @ # global
    >
    @ # local
    $ # memory(1)
    >
    @ # global
    >
    @ # local
    $ # memory(2)
    >
    @ # global
    >
    @ # local
    $ # memory(3)
    >
    @ # global
    >
    @ # local
    $ # memory(4)
    >

    + # for equals comparison
    >++ # for greater comparison
    >+++ # for greater or equals comparison; memory(7)
    # equals comparison
    >>[-]  # memory(9) temp1
    <[-]+ # memory(8) temp0
    =    # memory(1)
    -[

        # case greater or case greater or equals
        >>>>>>>>[-]
        <[-]+=
        -[
            # case greater or equals
            <
            [>>>>> >>>>> >+>+ <<<<< <<<<< <<-]
            >>>>> >>>>> >>
            [<<<<< <<<<< <<+>>>>> >>>>> >>-]
            <<<<< <<<<<

            [>>>>> >>>>>+>+ <<<<< <<<<< <-]
            >>>>> >>>>>>
            [<<<<< <<<<< <+>>>>> >>>>> >-]
            <<<<<
            # greater
            [-]
            >[-]
            >[-]
            =
            [ 
                >>>> >>>>+
                <<< <<<[- >>> >>>[-] >+ <<<<< <<]
                >>> >>>[- >>+ <<]
                >[- <<<<< <<+ >>>>> >>]
                <<<<< <<- <<- 
            ]
            
            +++++
            +++++
            +
            =
            # equals
            [->-<]+>[<->[-]]
            <
            # sum of greater and equals
            >[-]
            <<[>+>+<<-]
            >>[<<+>-]
            <
            [
                # true case
                [-]+++=!%
            ]
            # false case
            [-]++++=!%            
        ]
        >>>>>>>>[<<<<<<<<+>>>>>>>>-]
        <[
            # case greater
            [-]
            >[-]
            >[-]
            =
            [
                >>>>>>>>+
                <<<<<<[- >>>>>>[-] >+ <<<<<<<]
                >>>>>>[- >>+ <<]
                >[- <<<<<<<+ >>>>>>>]
                <<<<<<<- <<- 
            ]
            +++++
            +++++
            =
            [
                # true case
                [-]+++=!%
            ]
            # false case
            [-]++++=!%
            
        ]

        
    ]
    >>>>>>>>[<<<<<<<<+>>>>>>>>-]
    <[
        # case equals
        [-]+ # memory(8)
        >[-] # memory(9)
        =    # memory(0)
        [>>-<<-]
        >>
        [
            # false case
            [-]++++=!%
        ]
        >>>>>>>[<<<<<<<+>>>>>>>-]
        <[
            # true case
            [-]+++=!%
        ]
    ]

}