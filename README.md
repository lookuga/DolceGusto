# DolceGusto
Repo for Open Source Code and designs that fit the Krups Piccolo and other Dolce Gusto coffee machines

See full article here: https://lookuga.com/post/reverse-engineering-the-krups-piccolo-part-3-the-finale

ProtoBoard.ai
Contains multi-layer illustrator file, broken down (pcb, top wiring,bottom wiring, components etc)

KrupsPiccolo_OpenSource.ino
Contains the source code for the microcontroller

Instructions to Use

    Connect power to coffee machine
    Should start pulsing green
    On pressing the power button, should turn to yellow (means heating up)
    Once heated up should turn to solid green
    On switch to start pump, it should start to pump water (either hot or cold) and should be yellow (heating while pumping)
    Show red, if the capsule holder is not detected when trying to start pump