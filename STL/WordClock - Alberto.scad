letters = [
    ["C", "U", "A", "T", "R", "E", "O", "S"],
    ["N", "U", "S", "I", "E", "T", "V", "E"],
    ["C", "I", "N", "O", "C", "H", "O", "I"],
    ["D", "I", "O", "N", "C", "E", "Z", "S"],
    ["Y", "M", "E", "N", "O", "S", "M", "E"],
    ["C", "U", "A", "R", "T", "O", "D", "I"],
    ["V", "E", "I", "N", "T", "I", "E", "A"],
    ["D", "C", "I", "N", "E", "Z", "C", "O"],
];
cells = 8;

$fn=100;

font_name = "stencil std";
font_size = 6.5;

panel_extrude = 2;
letters_extrude = 1;

led_space = 3.26;
led_side = 5;

// grid
show_grid = true;
grid_width = 1.3;
grid_height = 5;

letter_size = led_space + led_side;
total_side = letter_size * cells;
init_column = (letter_size/2);
init_row = total_side - (letter_size/2);

difference() {
    cube (size=[total_side, total_side, panel_extrude]);

    x = 0;
    y = 0; 
    for( row = [0:cells-1]) {
        for( column = [0:cells-1]) {
            translate([0,0,panel_extrude-letters_extrude])
            linear_extrude(height=letters_extrude+1)
                translate([
                    init_column + (column*letter_size), 
                    init_row - (row*letter_size)
                ])
                text(letters[row][column],
                    font=font_name, size=font_size, 
                    halign="center", valign="center");
            
            x = x + led_side + led_space;
        }
        y = y + led_side + led_space;
    }
}


if(show_grid) {
    
    for (i = [0:cells]) {
        color("red", alpha=0.5)
        translate ([0, i*letter_size, grid_height])
        cube (size=[total_side, led_space, grid_width]);
        
        color("red", alpha=0.5)
        translate ([i*letter_size, 0, grid_height])
        cube (size=[led_space, total_side, grid_width]);
    }
}