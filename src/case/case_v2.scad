
//Konstanten
//Hoehe der Grundflaeche
baseHeight = 3; //mm
edgeWidth = 4;

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

PWRLEDHoleWidth = 5;

//Ausmase des hx711 Boards
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

module Base($fn) {
    
    edgeHeight = height - baseHeight;
    
    //Grundflaeche
    cylinder(h=baseHeight,r=diameter/2,center=false);
    //Rand
    translate([0,0,baseHeight]){
        difference() {
            cylinder(h=edgeHeight,r=diameter/2,center=false);
            cylinder(h=edgeHeight*3,r=(diameter/2)-edgeWidth,center=true);
        }
    }
}

//alles was aus Metall ist
module LoadCell() {
    //+0.01 ist nur ein Hack damit das Grafikflimmern aufhoert
    translate([0,0,0.01])
    color([1,0,0]){
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
    translate([-47.5,-yLCClamp/2,0]){
        cube(size=[xLoadCell,yLCClamp,30], center=false);
    }
    translate([-47.5+xLoadCell-xLCClamp-2,-yLCClamp/2-1,0]){
        cube(size=[xLCClamp+2,yLCClamp+2,30], center=false);
    }
}

module Battery(){
    color([1,0,0]){
        translate([BatteryPosX,BatteryPosY,-0.1]){
            cube(size=[xBattery,yBattery,zBattery+0.1], center=false);
        }
    }
}

module BatteryBB(){
    translate([BatteryPosX,BatteryPosY,-0.1]){
        cube(size=[xBattery,yBattery,30], center=false);
    }
}


module HX711(){
    color([1,0,0]){
        translate([HX711posX,HX711posY,baseHeight+HX711distToBase]){
            cube(size=[xHX711,yHX711,zHX711], center=false);
        }
    }
}

module HX711Mount(){
    translate([HX711posX,HX711posY,baseHeight]){
        cube(size=[xHX711,yHX711,HX711distToBase], center=false);
    }
}

module HX711BB(){
    translate([HX711posX,HX711posY,0]){
        cube(size=[xHX711,yHX711,30], center=false);
    }
}

module BoardLEDs($fn, width){
    translate([0,0,baseHeight+0.001]){
        intersection(){
            translate([-width/2,yBottom2+10,0]){
                cube(size=[width,diameter/2,zBoard], center=false);
            }
            cylinder(h=zBoard,r=diameter/2,center=false);
        }
    }   
}

module Board($fn){
    color([1,0,0]){
        rotate([0,0,20]){
            BoardLEDs(feinheit,LEDHoleWidth);
        }
        rotate([0,0,-20]){
            BoardLEDs(feinheit,LEDHoleWidth);
        }
        rotate([0,0,-50]){
            BoardLEDs(feinheit,PWRLEDHoleWidth);
        }
        difference() {
            //+0.001 ist nur ein Hack damit das Grafikflimmern aufhoert
            translate([0,0,baseHeight+0.001]){
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
        }
    }
}

//Freiheitsgrade
//Durchmesser in mm, Hoehe (mit Rand) in mm, Feinheit
diameter = 110;
height = 15;
feinheit = 100;

BatteryPosX = -25;
BatteryPosY = -44;

HX711posX = 14;
HX711posY = -14;
HX711distToBase = 3;

yBottom1 = 12;
yBottom2 = 20;

difference() {
    Base(feinheit);
    union(){
        BatteryBB();
        BoardBB();
    }
}

LoadCellMount();
HX711Mount();
Fill(feinheit);


//Battery();
//LoadCell();
//HX711();
//Board(feinheit);


