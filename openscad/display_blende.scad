dispW = 25;
dispH = 13.7;

dispYoff = 0;

holeW = 110;
holeH = 23;

dist = 2.54 * 10;

kragenOut = 7;
thicknessOut = 1;

kragenIn = 2;
thicknessIn = 1;
stegWidth = 2;


borderW = holeW + kragenOut;
borderH = holeH + kragenOut;

depth = 3;
    //font();
difference() {
    main();
    font();
}




module font() {
    startx = -43;
    font="Bahnschrift";
    translate([0,-0.5,0.3]) linear_extrude(0.2) {
        translate([startx,9.5,0]) text("CH 1", size=4, font=font);
        translate([startx + dist,9.5,0]) text("CH 2", size=4, font=font);
        translate([startx + dist * 2,9.5,0]) text("CH 3", size=4, font=font);
        translate([startx + dist * 3,9.5,0]) text("CH 4", size=4,font=font);
    }
}



module main() {
    difference() {
        union() {
            cube([borderW, borderH, thicknessOut], true);
            translate([0,0,-2.5]) cube([102, 13, 5], true);
            difference() {
                translate([0,0,-depth/2]) cube([holeW, holeH, depth], true);        
                translate([0,16,-3]) cube([102,20,3],true);
            }    }
        translate([-dist/2-+dispW,0,0]) displayCut();
    }
}

module displayCut() {
    for(i=[0:1:3]) {
        translate([-0.7 + i * dist, 0, 0]) cube([dispW, dispH, 50], true);
    }
}