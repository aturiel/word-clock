
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
max = 66;   // led side?
s = 66/8;
d_h = 5;    // led dividers?
w_h = 10;   // face height
b_h = 5;    // dividers height
c_h = 35;   // base height
letter_depth = w_h+1;

diffusers_height = 2.25;

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
		dividers(1.8);
	}	
    translate([1,1,diffusers_height]) cube([max,max,0.30]);
}

module standoff()
{
	difference()
	{
		cylinder(d1=7,d2=5,h=28);
		translate([0,0,19])	cylinder(d=2.5, h=10);
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
		
		translate([-1.5,-1.5,0]) 			cylinder(d=5.5,h=c_h+28, center=true);	// ll 
		translate([max+1+1.5,-5+3.5,0]) 	cylinder(d=5.5,h=c_h+28, center=true);	// lr
		translate([-1.5,max+1+1.5,0]) 		cylinder(d=5.5,h=c_h+28, center=true);	// ul
		translate([max+1+1.5,max+1+1.5,0])	cylinder(d=5.5,h=c_h+28, center=true);	// ur 
        
        // ventilation grill
        tol = 0.2;
        translate([12,15.5,0])	
             difference(){
               linear_extrude(height = 10)
               cube([16,30,10]);
               for (x=[0:4:16]){
                 for (y=[0:4:32]){
                   translate([x,y,-tol/2]){
                     cylinder(r=1.5,h=10+tol,$fn=180);
                   }
                 }
               }
             }
            
            
		// FTDI cutout
		//#translate([18+6+5+6,-5,15])	cube([19, 8, 8]); 	// bottom

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
		translate([4,(max/2),0])
			 cylinder(d=8,h=6, center=true);
		
		/*translate([12,26,0.2]) rotate([0,180,0])
					linear_extrude(height = 0.2)
					text(
						"+5vdc", 
						font = f,
						size=6, 
						$fn=20, 
						valign="center", 
						halign = "center"
					);*/
	}
    
	// standoffs for AC-DC
	translate([5+5+9, 8    , 2]) 	standoff();
	translate([5+5+9, 8 +50, 2]) 	standoff();
	
	// standoffs for ESP 
	translate([5+5+28    , 6    , 2]) 	standoff();
	translate([5+5+28    , 6 +52, 2]) 	standoff();
	translate([5+5+28 +25, 6    , 2])	standoff();
	translate([5+5+28 +25, 6 +52, 2]) 	standoff();
    
    // debug modules
    //#translate([6, 13, 8])	cube([28, 40, 23]); 	// AC-DC
    //#translate([35, 3, 7])	cube([31, 60, 23]); 	// ESP
}

module dot_pattern(size, line_size, line_space) {
    rad = line_space / 2;
    y_offset = sqrt((rad + line_size / 2) * (rad + line_size / 2) * 4 - (rad + line_size / 2) * (rad + line_size / 2));
    num_x = ceil(size / rad / 2) * 1.42;
    num_y = ceil(size / y_offset) * 1.42;
    difference() {
        square([size * 1.42, size * 1.42], center = true);
        for(y = [floor(-num_y / 2) : ceil(num_y / 2)]) {
            odd = (y % 2 == 0) ? 0 : rad + line_size / 2;
            for(x = [floor(-num_x / 2) : ceil(num_x / 2)]) {
                translate([x * (rad + line_size / 2) * 2 + odd, y * y_offset]) {
                    rotate(30) {
                        circle(d=line_space);
                    }
                }
            }
        }
    }
}


// uncomment for clock face
translate([-15,0,10]) rotate([0,180,0]) clock_face();

// uncomment for led diffusers
//translate([0,-80,0]) diffusers();

// uncomment for back of case
//case_back();

