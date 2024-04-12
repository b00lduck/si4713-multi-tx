w = 159;
h = 82;
t = 0.4;
$fn = 100;

dsx = 2.54 * 60;
dsy = 2.54 * 30;

sh = 4 + t;

translate([-w/2,-h/2,0]) cube([w,h,t]);


translate([-dsx/2,-dsy/2,0]) stud();
translate([-dsx/2,dsy/2,0]) stud();
translate([0,dsy/2,0]) stud();
translate([2.54*4,-dsy/2,0]) stud();
translate([-2.54*8,-dsy/2,0]) stud();
translate([dsx/2,dsy/2,0]) stud();
translate([dsx/2,-dsy/2,0]) stud();


module stud() {
    
    difference() {
        cylinder(sh,3,3);
        cylinder(sh,1,1);
    }    
}