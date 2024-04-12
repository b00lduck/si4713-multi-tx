$fn = 100; 
r1 = 25/2;
r2 = 19/2;

l1 = 2;
l2 = 4;
l3 = 2;

lges = l1+l2+l3;

cylinder(2,r1,r1);
difference() {
    union() {
        translate([0,0,6]) cylinder(2,r2+1,r2);
        cylinder(lges,r2,r2);
    }

    for (i=[0:45:360])
    rotate([0,0,i]) cube([40,2,20],center=true);
    
    cylinder(lges,r2-0.5,r2-0.5);

}