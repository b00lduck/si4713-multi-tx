$fn = 100;

width = 437;
height = 43;
thickness = 2;

half1Width = 213;
half2Width = width - half1Width;

distPlate = 3;

connectorWidth = 10;
connectorDepth = 20;
connectorHeight = height - (distPlate*2);

holedistY = 35;
holedistX = 428;

half1();

module half1() {
    // left
    difference() {
        half(half1Width);
        translate([half1Width-(connectorWidth/2),height,10+thickness]) rotate([90,0,0]) cylinder(5,0.9,0.9);
        translate([half1Width-(connectorWidth/2),5,10+thickness]) rotate([90,0,0]) cylinder(5,0.9,0.9);

        translate([30,height/2,0]) cylinder(10,4,4);

        // bnc
        for(x=[0:25:80]) {
            translate([x+100,height/2,0]) cylinder(10,5,5);
        }
    }
}

module half2() {
    // right
    translate([0,0,0]) 
    difference() {
        rotate([0,0,0]) half(half2Width);

        // cinch
        for(y=[-7,7])
        for(x=[0:20:60]) {
            translate([x+100,height/2+y,0]) cylinder(10,4,4);
        }

    }
}

module half(w) {
    difference() {
        union() {
            cube([w, height, thickness]);
            translate([25,distPlate,0]) cube([w-30, 4, 7]);
            translate([25,height-4-distPlate,0]) cube([w-30, 4, 7]);
            translate([w - connectorWidth,distPlate,0]) cube([connectorWidth, connectorHeight, connectorDepth]);
        }
        translate([w-20,height-10,connectorDepth/2]) rotate([0,90,0]) cylinder(40,2,2);
        translate([w-20,10,connectorDepth/2]) rotate([0,90,0]) cylinder(40,2,2);

        translate([4,(height-holedistY)/2,0]) rotate([0,0,0]) cylinder(40,2,2);
        translate([4,(height+holedistY)/2,0]) rotate([0,0,0]) cylinder(40,2,2);        
    }
    
}