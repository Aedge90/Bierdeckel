
//Konstanten
//Hoehe der Grundflaeche
baseHeight = 3; //mm
edgeWidth = 3;

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
    translate([-47.5,-yLCClamp/2,baseHeight]){
        cube(size=[xLCClamp,yLCClamp,dist+zLoadCell-(zLCClamp-1)], center=false);
    }
    translate([-47.5+xLCClamp,-yLCClamp/2,baseHeight]){
        trapezoid(xLoadCell-(2*xLCClamp),17,dist+zLoadCell,
        (yLCClamp-yLoadCell)/2 - 1);
    }
    translate([-47.5+xLCClamp,yLoadCell/2 + 1,baseHeight]){
        trapezoid(xLoadCell-(2*xLCClamp),17,dist+zLoadCell,
        (yLCClamp-yLoadCell)/2 - 1);
    }
}

module Battery(posX, posY){
    translate([posX,posY,-0.1]){
        cube(size=[xBattery,yBattery,zBattery+0.1], center=false);
    }
}

module HX711(posX, posY){
    translate([posX,posY,baseHeight]){
        cube(size=[xHX711,yHX711,zHX711], center=false);
    }
}

module Board(yBottom1, yBottom2, $fn){
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

//Freiheitsgrade
//Durchmesser in mm, Hoehe (mit Rand) in mm, Feinheit
diameter = 110;
height = 15;
feinheit = 100;

#LoadCell();
LoadCellMount();
difference() {
    Base(feinheit);
    #Battery(-25, -45);
}
#HX711(14, -15);
color([0,1,1,0.5]){
    Board(11,20,feinheit);
}



