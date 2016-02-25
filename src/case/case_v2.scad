
//Konstanten
//Hoehe der Grundflaeche
baseHeight = 3; //mm
edgeWidth = 5;

//Ausmase der Load Cell
xLoadCell = 55;
yLoadCell = 13;
zLoadCell = 8;

//Ausmase der Metallbuegel
xLCClamp = 15;
yLCClamp = 37;
zLCClamp = 6;

//Ausmase des Batteriefachs
xBattery = 50;
yBattery = 24.5;
zBattery = 13;

//Ausmase des hx711 Moduls
xHX711 = 33.5;
yHX711 = 20.5;
zHX711 = 3;

LEDHoleWidth = 22;

//maximaler Durchmesser des LED Models
LEDOuterRadius = 3.85;
LEDInnerRadius = 3;

PWRLEDHoleWidth = 5;

//Ausmase des Boards
zBoard = 12 ;  //TODO nachmessen!


//Lasse 4 mm Platz zum Boden damit sich die Load Cell verbiegen kann
dist = 4;


module trapezoid(width_base, width_top,height,thickness) {
    translate([0,thickness,0]){
    rotate([90,0,0]){
  linear_extrude(height = thickness) polygon(points=[[0,0],[width_base,0],[width_base-(width_base-width_top),height],[0,height]], paths=[[0,1,2,3]]); 
    }
}
}

//alles was aus Metall ist
module LoadCell() {
    //linke Seite der Load Cell muss 47.5mm vom Mittelpunkt entfernt sein
    //damit das Loch genau im Mittelpunkt liegt
    translate([-47.5,-yLoadCell/2,baseHeight+dist]){
        cube(size=[xLoadCell,yLoadCell,zLoadCell], center=false);
    }
    //erster Metallhalter ist ca. 1mm dick ueber der Load Cell
    translate([-47.5,-yLCClamp/2,baseHeight+dist+zLoadCell-(zLCClamp-1)]){
        cube(size=[xLCClamp,yLCClamp,zLCClamp], center=false);
    }
    //zweiter Metallhalter ist ca. 1mm dick unter der Load Cell
    translate([-47.5+xLoadCell-xLCClamp,-yLCClamp/2,baseHeight+dist-1]){
        cube(size=[xLCClamp,yLCClamp,zLCClamp], center=false);
    }
}

module LoadCellMount(){
    //Halterung fuer ersten Metallhalter
    difference(){
        translate([-47.5,-yLCClamp/2,baseHeight]){
            cube(size=[xLCClamp,yLCClamp,dist+zLoadCell-(zLCClamp-1)], center=false);
        }
        translate([-47.5,-yLoadCell/2,baseHeight+dist]){
            cube(size=[xLoadCell,yLoadCell,zLoadCell], center=false);
        }
    }
    translate([-47.5+xLCClamp,-yLCClamp/2,baseHeight]){
        trapezoid(xLoadCell-(2*xLCClamp),17,dist+zLoadCell,
        (yLCClamp-yLoadCell)/2 - 1);
    }
    translate([-47.5+xLCClamp,yLoadCell/2 + 1,baseHeight]){
        trapezoid(xLoadCell-(2*xLCClamp),17,dist+zLoadCell,
        (yLCClamp-yLoadCell)/2 - 1);
    }
    //Mitte
    translate([-47.5+xLCClamp,-yLoadCell/2 - 1,baseHeight]){
        trapezoid(xLoadCell-(2*xLCClamp),0,dist,yLoadCell + 1 + 1);
    }
}

module LoadCellBB(){
    translate([-47.5,-yLCClamp/2,baseHeight]){
        cube(size=[xLoadCell,yLCClamp,30], center=false);
    }
    translate([-47.5+xLoadCell-xLCClamp-2,-yLCClamp/2-1,baseHeight]){
        cube(size=[xLCClamp+2,yLCClamp+2,30], center=false);
    }
}

module Battery(){
    translate([BatteryPosX,BatteryPosY,0]){
        cube(size=[xBattery,yBattery,zBattery], center=false);
    }
}

module BatteryBB(){
    translate([BatteryPosX,BatteryPosY,-0.1]){
        cube(size=[xBattery,yBattery,30], center=false);
    }
}


module HX711(){
    translate([HX711posX,HX711posY,baseHeight+HX711distToBase]){
        cube(size=[xHX711,yHX711,zHX711], center=false);
    }
}

module HX711Mount(){
    translate([HX711posX,HX711posY,baseHeight]){
        cube(size=[xHX711,yHX711,HX711distToBase], center=false);
    }
}

module HX711BB(){
    translate([HX711posX,HX711posY,baseHeight]){
        cube(size=[xHX711,yHX711,30], center=false);
    }
}

module BoardLED($fn, width){
    translate([-width/2,diameter/2.4,height-LEDHoleZ+0.001]){
        cube(size=[width,diameter/9,LEDHoleZ], center=false);
    }   
}

module LED($fn){
    translate([0,diameter/2 - edgeWidth,height-LEDHoleZ+LEDInnerRadius/2]){
        rotate([-90,90,0]){
            cylinder(h=1,r=LEDOuterRadius/2,center=false);
            cylinder(h=5.3-LEDInnerRadius/2,r=LEDInnerRadius/2,center=false);
            translate([0,0,5.3-LEDInnerRadius/2]){
                sphere(r = LEDInnerRadius/2);
            }
        }
    }
}

module BoardLEDsBB() {
    rotate([0,0,20]){
        BoardLED(feinheit,LEDHoleWidth);
    }
    rotate([0,0,-20]){
        BoardLED(feinheit,LEDHoleWidth);
    }
    rotate([0,0,-50]){
        BoardLED(feinheit,PWRLEDHoleWidth);
    }
}

module BoardLEDs() {
    color([1,1,0]){
        rotate([0,0,11]){
            LED(21);
        }
        rotate([0,0,17]){
            LED(21);
        }
        rotate([0,0,23]){
            LED(21);
        }
        rotate([0,0,29]){
            LED(21);
        }
    }
    color([1,0,0]){
        rotate([0,0,-11]){
            LED(21);
        }
        rotate([0,0,-17]){
            LED(21);
        }
        rotate([0,0,-23]){
            LED(21);
        }
        rotate([0,0,-29]){
            LED(21);
        }
    }
    color([0,1,0]){
        rotate([0,0,-50]){
            LED(21);
        }
    }
}

module BoardMount(){
    translate([-10,23,baseHeight]){
        cube(size=[xHX711,yHX711,BoardDistToBase], center=false);
    }
}

module Board($fn){
    difference() {
        translate([0,0,baseHeight]){
            cylinder(h=zBoard,r=diameter/2-edgeWidth,center=false);
        }
        union(){
            translate([-100,-100+yBottom1,0.1]){
                cube(size=[200,100,30]);
            }
            translate([-100,-100+yBottom2,0.1]){
                cube(size=[100+14,100,30]);
            }
        }
    }  
}

module BoardBB($fn){
    scale([1.001,1.001,2]){
        translate([0,0,-baseHeight/2]){
            Board(feinheit);
        }
    }
}


module Fill($fn){
    difference(){
        cylinder(h=height,r=diameter/2,center=false);
        union(){
            HX711BB();
            BatteryBB();
            LoadCellBB();
            BoardBB(feinheit);
            BoardLEDsBB();
        }
    }
}

//Freiheitsgrade
//Durchmesser in mm, Hoehe (mit Rand) in mm, Feinheit
diameter = 108;
height = 15;
feinheit = 200;

BatteryPosX = -25;
BatteryPosY = -44;

HX711posX = 14;
HX711posY = -14;
HX711distToBase = 3;

yBottom1 = 12;
yBottom2 = 20;

LEDHoleZ = 3.5;

BoardDistToBase = 3;

//color([0,1,0]){
    LoadCellMount();
    HX711Mount();
    BoardMount();
    Fill(feinheit);
//}
    
color([0,0,1]){
    //Battery();
    //LoadCell();
    //HX711();
    translate([0,0,+0.001]){
    //    Board(feinheit);
    }
}

BoardLEDs();



