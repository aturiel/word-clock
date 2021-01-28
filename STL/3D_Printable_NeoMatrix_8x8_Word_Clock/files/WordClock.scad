
letters = [ 
    ["D", "I", "O", "N", "C", "E", "Z", "S"],
    ["N", "U", "S", "I", "E", "T", "V", "E"],
    ["C", "I", "N", "O", "C", "H", "O", "I"],
    ["C", "U", "A", "T", "R", "E", "O", "S"],
    ["Y", "M", "E", "N", "O", "S", "M", "E"],
    ["C", "U", "A", "R", "T", "O", "D", "I"],
    ["V", "E", "I", "N", "T", "I", "E", "A"],
    ["D", "C", "I", "N", "E", "Z", "C", "O"],    
];

$fn = 64;
max = 63; 
s = 63/8;
d_h = 5;
w_h = 10;
b_h = 5;
c_h = 23;
letter_depth = w_h+1;

diffusers_height = 2.2;

f = "stencil std";

module face()
{
	for (r = [0:7])
	{
		for (c = [0:7])
		{
			#translate([r*s + s/2+.4, c*s+s/2+.5, 0])
				linear_extrude(height = letter_depth+1)
					text(
						letters[7-c][r], 
						font = f,
						size=6, 
						$fn=20, 
						valign="center", 
						halign = "center"
					);
		}
	}
}

module case_front()
{
	difference()
	{
		union()
		{
			translate([-5,-5,0]) 	cube([5,max+6+5, w_h]);	// left side
			translate([max+1,-5,0])	cube([5,max+6+5, w_h]); // right side 
			translate([0,max+1,0]) 	cube([max+2, 5, w_h]);	// top
			translate([-1,-5,0])	cube([max+3, 5, w_h]); 	// bottom
		}
		
		// mounting holes
		
		translate([-1.5,-1.5,0]) 			cylinder(d=2.4,h=w_h-2);	// ll 
		translate([max+1+1.5,-5+3.5,0]) 	cylinder(d=2.4,h=w_h-2);	// lr
		translate([-1.5,max+1+1.5,0]) 		cylinder(d=2.4,h=w_h-2);	// ul
		translate([max+1+1.5,max+1+1.5,0])	cylinder(d=2.4,h=w_h-2);	// ur 
	}
}

module dividers(w=1)
{
	for (x = [0:8])
	{
		translate([s*x, 1, 0]) cube([w,max,d_h]);
		translate([0, s*x,0]) cube([max+1, w, d_h]);
	}
}

module clock_face()
{
	difference()
	{
		union()
		{
			translate([0,0,w_h-1]) cube([max+1,max+1,1]);
			translate([0,0,b_h]) dividers();
		}
		face();
	}
	case_front();
}

module diffusers()
{
	difference()
	{
		translate([1,1,0]) cube([max,max,diffusers_height]);
		dividers(1.8 );
	}	
}

module standoff()
{
	difference()
	{
		cylinder(d1=7,d2=5,h=7);
		translate([0,0,2])	cylinder(d=2.5, h=7);
	}
}

module case_back()
{
	difference()
	{
		union()
		{
			cube([max+1,max+1,2]);
			translate([-5,-5,0]) 		cube([5,max+6+5, c_h]);	// left side
			translate([max+1,-5,0])		cube([5,max+6+5, c_h]); // right side 
			translate([0,max,0]) 	    cube([max+2, 6, c_h]);	// top
			translate([-1,-5,0])		cube([max+2, 6, c_h]); 	// bottom
		}
		
		// mount holes 
		translate([-1.5,-1.5,0]) 			cylinder(d=3,h=c_h+4);	// ll 
		translate([max+1+1.5,-5+3.5,0]) 	cylinder(d=3,h=c_h+4);	// lr
		translate([-1.5,max+1+1.5,0]) 		cylinder(d=3,h=c_h+4);	// ul
		translate([max+1+1.5,max+1+1.5,0])	cylinder(d=3,h=c_h+4);	// ur 
		
		translate([-1.5,-1.5,0]) 			cylinder(d=5.5,h=4);	// ll 
		translate([max+1+1.5,-5+3.5,0]) 	cylinder(d=5.5,h=4);	// lr
		translate([-1.5,max+1+1.5,0]) 		cylinder(d=5.5,h=4);	// ul
		translate([max+1+1.5,max+1+1.5,0])	cylinder(d=5.5,h=4);	// ur 
		
		// FTDI cutout
		#translate([18+6+5+6,-5,15])	cube([19, 8, 8]); 	// bottom

		// Time Adjust Button Holes
        /*
		translate([18+30+5+5,10+12.5+1.5,0])	cylinder(d=5,h=5);
		translate([18+30+20,10+12+1.5,.2]) 	rotate([0,180,0])
			linear_extrude(height = 0.2)
					text(
						"+M", 
						font = f,
						size=6, 
						$fn=20, 
						valign="center", 
						halign = "center"
					);
        
		translate([18+30+5+5,10+30+1.5,0])	cylinder(d=5,h=5);
		translate([18+30+20,10+30+1.5,.2])	rotate([0,180,0])
					linear_extrude(height = 0.2)
					text(
						"+H", 
						font = f,
						size=6, 
						$fn=20, 
						valign="center", 
						halign = "center"
					);
		*/

		// power socket cutout
		translate([12,13,0])
			 cylinder(d=12,h=16);
		
		translate([12,26,0.2]) rotate([0,180,0])
					linear_extrude(height = 0.2)
					text(
						"+5vdc", 
						font = f,
						size=6, 
						$fn=20, 
						valign="center", 
						halign = "center"
					);
	}
	
	// standoffs for pc 
	translate([18+5+5, 10, 2]) 			standoff();
	translate([18+5+5, 10+46, 2]) 		standoff();
	translate([18+30+5+5, 10, 2]) 		standoff();
	translate([18+30+5+5, 10+46, 2]) 	standoff();
}

// uncomment for clock face
//translate([-15,0,10]) rotate([0,180,0]) clock_face();

// uncomment for led diffusers
//translate([0,-80,0]) diffusers();

// uncomment for back of case
case_back();

