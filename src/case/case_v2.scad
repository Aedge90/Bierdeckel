
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

//Lasse 4 mm Platz zum Boden damit sich die Load Cell verbiegen kann
dist = 4;

module Base(diameter, height, $fn) {
    
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

//Freiheitsgrade
//Durchmesser in mm, Hoehe (mit Rand) in mm, Feinheit
#LoadCell();
LoadCellMount();
difference() {
    Base(110, 15, 100);
    #Battery(-25, -45);
}
#HX711(14, -15);



