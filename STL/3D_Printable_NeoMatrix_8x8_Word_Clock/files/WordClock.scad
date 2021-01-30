
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

font_face = "stencil std";
font_brand = "futura";

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
						font = font_face,
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

module standoff( height, width_bottom, width_top)
{
	difference()
	{
		cylinder(d1=width_bottom,d2=width_top,h=height);
		translate([0,0,height-10+1])	cylinder(d=2.5, h=10);
	}
}

module case_back()
{
	difference()
	{
		union()
		{
			cube([max+1, max+1, 2]);
			translate([   -5,  -5, 0]) 		cube([5    , max+6+5, c_h]);	// left side
			translate([max+1,  -5, 0])		cube([5    , max+6+5, c_h]);    // right side 
			translate([    0, max, 0]) 	    cube([max+2,       6, c_h]);	// top
			translate([   -1,  -5, 0])		cube([max+2,       6, c_h]); 	// bottom
		}
		
		// mount holes 
        screwDeep = 17;
		translate([     -1.5,      -1.5, 0]) 	cylinder(d=3,h=c_h+1);	// ll 
		translate([max+1+1.5,    -5+3.5, 0]) 	cylinder(d=3,h=c_h+1);	// lr
		translate([     -1.5, max+1+1.5, 0]) 	cylinder(d=3,h=c_h+1);	// ul
		translate([max+1+1.5, max+1+1.5, 0])	cylinder(d=3,h=c_h+1);	// ur 
		
		translate([     -1.5,      -1.5, 0]) 	cylinder(d=5.5,h=c_h-screwDeep);	// ll 
		translate([max+1+1.5,    -5+3.5, 0]) 	cylinder(d=5.5,h=c_h-screwDeep);	// lr
		translate([     -1.5, max+1+1.5, 0]) 	cylinder(d=5.5,h=c_h-screwDeep);	// ul
		translate([max+1+1.5, max+1+1.5, 0])	cylinder(d=5.5,h=c_h-screwDeep);	// ur 
        
        // ventilation grill
        tol = 0.2;
        translate([14,18,0])	
             difference(){
               linear_extrude(height = 10)
               cube([16,30,10]);
               for (x=[0:5:15]){
                 for (y=[0:5:30]){
                   translate([x,y,-tol/2]){
                     cylinder(r=2,h=10+tol,$fn=180);
                   }
                 }
               }
             }
            
        // slots for LED connector
		#translate([max-12,max-0.5,c_h-9])	cube([12, 4.5, 10]); 	
		#translate([1,0-3,c_h-9])	cube([12, 4.5, 10]);
            
		// power socket cutout
		translate([5,(max/2),0])
			 cylinder(d=9,h=6, center=true);
		
        // brand
		translate([max/2+15,max/2,0.3]) rotate([180,0,90])
					linear_extrude(height = 0.3)
					text(
						"Word-Clock", 
						font = font_brand,
						size=6, 
						$fn=20, 
						valign="center", 
						halign = "center"
					);
	}
    
    // debug modules
    //#translate([6, 13, 3])	cube([28, 40, 23]); 	// AC-DC
    //#translate([35, 3, 5])	cube([31, 60, 18]); 	// ESP
    
    // strandoffs
    width_bottom = 8;
    width_top = 8;
    
	// standoffs for AC-DC
    acdc_height = 24;
    acdc_length = 50;
    acdc_x = 5+5+9;
    acdc_y = 8;
	translate([acdc_x, acdc_y             , 2]) standoff(acdc_height, width_bottom, width_top);
	translate([acdc_x, acdc_y+acdc_length , 2]) standoff(acdc_height, width_bottom, width_top);
    
	// standoffs for ESP 
    esp_height = 21;
    esp_width = 25;
    esp_length = 52;
    esp_x = 5+5+28;
    esp_y = 6;
	translate([esp_x          , esp_y             , 2]) standoff(esp_height, width_bottom, width_top);
	translate([esp_x+esp_width, esp_y             , 2]) standoff(esp_height, width_bottom, width_top);
	translate([esp_x          , esp_y+esp_length  , 2]) standoff(esp_height, width_bottom, width_top);
	translate([esp_x+esp_width, esp_y+esp_length  , 2]) standoff(esp_height, width_bottom, width_top);
    
    // aditional supports for AC-DC
	#translate([acdc_x-(width_top/2), 0    , 2])    cube([8,8,14]); //OK
	#translate([acdc_x-(width_top/2), max-9, 2]) 	cube([8,9,14]); //OK

    // aditional supports for ESP 
	#translate([esp_x-(width_top/2),     0, 2])    cube([8,7,12]); //OK
	#translate([esp_x-(width_top/2), max-9, 2])    cube([8,9,12]); //OK
    
	#translate([esp_x-(width_top/2)+esp_width    , 0, 2])   cube([9,4.5,21]);
	#translate([esp_x-(width_top/2)+esp_width+5.5, 0, 2]) 	cube([4,8,21]);
    
	#translate([esp_x-(width_top/2)+esp_width    , esp_y+esp_length+1.5, 2]) cube([9,8,21]);
	#translate([esp_x-(width_top/2)+esp_width+5.5, esp_y+esp_length-3  , 2]) cube([4,5,21]);
}

// uncomment for clock face
//translate([-15,0,10]) rotate([0,180,0]) clock_face();

// uncomment for led diffusers
//translate([0,-80,0]) diffusers();

// uncomment for back of case
case_back();

