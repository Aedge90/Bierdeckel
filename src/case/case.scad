module bierdeggl()
{
	// Helper for view, change to 0 when printing
	dview = 0.1;
	// Load Cell
	lch = 1.6;
	lcw = 1.6;
	lcl = 5.5;
	// Bottom
	bh = 0.5;
	bd = 10.7;
	br = bd/2;
	d = 0.25;
	cr = 1;
	

	cylinder(h = bh, d = bd, center = false);
	translate([lcl/2-cr,0,bh])
		cylinder(h = d, r1 = br-lcl/2+cr, r2 = cr, center = false);
	translate([0,0,bh+d+lch/2])
		cube(size = [lcl,lcw,lch], center = true);

	translate([-lcl/2+cr,0,bh+d+lch])
		cylinder(h = d, r2 = br-lcl/2+cr, r1 = cr, center = false);
	translate([0,0,bh+2*d+lch])
		cylinder(h = bh, d = bd, center = false);

	difference() {
  		cylinder(h = bh+d+lch, r = br);
		translate([0, 0, 0])
    		cylinder(h = bh+d+lch+dview, r = br-1);
	}
}
bierdeggl();