#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

#define INFO(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;

using std::cout;
using std::endl;
using std::string;

int findMin(int[], int, int&);

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

int* createSeam(int length) {
  // TODO(student): create a seam
  //INFO(length);

  int* seam = new int[length];
  return seam;
}

void deleteSeam(int* seam) {
    // TODO(student): delete a seam
    delete [] seam;
    //INFO(seam);
}

bool loadImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): load an image
    // INFO(filename);
    // INFO(image);
    // INFO(width);
    // INFO(height);

    string file;
    string type;
    int w;
    int h;
    int colorMax;
  
    std::fstream ifs(filename);

    if(!ifs.is_open()){
      cout << "Error: failed to open input file - " << filename << endl;
      return false;
    }else{

      //parse and store header
      ifs >> type;
      ifs >> w >> h;
      ifs >> colorMax;

      if(type != "P3"){
        cout << "Error: type is " << type << " instead of P3" << endl;
        return false;
      }

      if(w != width){
        cout << "Error: input width (" << width << ") does not match value in file (" << w << ")" << endl;
        return false;
      }else if(h != height){
        cout << "Error: input height (" << height << ") does not match value in file (" << h << ")" << endl;
        return false;
      }

      //implement non integer value check

      //get pixel values
      while(!ifs.eof()){
        for(int i = 0; i < height; i++){
          for(int j = 0; j < width; j++){
            ifs >> image[j][i].r;
            ifs >> image[j][i].g;
            ifs >> image[j][i].b;

            if(image[j][i].r > 255 || image[j][i].r < 0){
              cout << "Error: invalid color value " << image[j][i].r << endl;
              return false;
            }
            if(image[j][i].g > 255 || image[j][i].g < 0){
              cout << "Error: invalid color value " << image[j][i].g << endl;
              return false;
            }
            if(image[j][i].b > 255 || image[j][i].b < 0){
              cout << "Error: invalid color value " << image[j][i].b << endl;
              return false;
            }
          }
        }
        if(ifs.fail()){
          cout << "Error: not enough color values" << endl;
          return false;
        }

        int extraVal;
        ifs >> extraVal;

        if(!ifs.fail()){
          cout << "Error: too many color values" << endl;
          return false;
        }
      }
    }

   
  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): output an image
    // INFO(filename);
    // INFO(image);
    // INFO(width);
    // INFO(height);

    std::ofstream ofs(filename);

    ofs << "P3" << endl;
    ofs << width << " " << height << endl;
    ofs << 255 << endl;

    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
        ofs << image[j][i].r << " ";
        ofs << image[j][i].g << " ";
        ofs << image[j][i].b << " ";
      }
    }

    ofs << endl;

  return true;
}

int energy(Pixel** image, int column, int row, int width, int height) { 
    // TODO(student): compute the energy of a pixel
   /* INFO(image);
    INFO(column);
    INFO(row);
    INFO(width);
    INFO(height);*/
  

    int Rx, Gx, Bx;
    int Ry, Gy, By;

    int deltX, deltY, energy;
    int tempThing;

    tempThing = row;
    row = column;
    column = tempThing;
  
    /*
    cout << "(" << row << "," << column << ") ";
    cout << image[row][column].r << " ";
    cout << image[row][column].g << " ";
    cout << image[row][column].b << "  ";

    if(row == 2){
      cout << endl;
    }*/
  
    
    if(row != 0 && row != width-1){
      //cout << "choice 1" << endl;      
      Rx = image[row+1][column].r - image[row-1][column].r;
      Gx = image[row+1][column].g - image[row-1][column].g;
      Bx = image[row+1][column].b - image[row-1][column].b;
    }

    if(column != 0 && column != height-1){
      //cout << "choice 2" << endl;
      Ry = image[row][column+1].r - image[row][column-1].r;
      Gy = image[row][column+1].g - image[row][column-1].g;
      By = image[row][column+1].b - image[row][column-1].b;
    }

    if(row == 0){
      //cout << "choice 3" << endl;
      Rx = image[row+1][column].r - image[width-1][column].r;
      Gx = image[row+1][column].g - image[width-1][column].g;
      Bx = image[row+1][column].b - image[width-1][column].b;
    }

    if(column == 0){
      //cout << "choice 4" << endl;
      Ry = image[row][column+1].r - image[row][height-1].r;
      Gy = image[row][column+1].g - image[row][height-1].g;
      By = image[row][column+1].b - image[row][height-1].b;
      
    }

    if(row == width-1){
      //cout << "choice 5" << endl;
      Rx = image[0][column].r - image[row-1][column].r;
      Gx = image[0][column].g - image[row-1][column].g;
      Bx = image[0][column].b - image[row-1][column].b;
      

    }

    if(column == height - 1){
      //cout << "choice 6" << endl;
      Ry = image[row][0].r - image[row][column-1].r;
      Gy = image[row][0].g - image[row][column-1].g;
      By = image[row][0].b - image[row][column-1].b;
    }

    deltX = pow(Rx,2) + pow(Gx,2) + pow(Bx,2);
    deltY = pow(Ry,2) + pow(Gy,2) + pow(By,2);

    energy = deltX + deltY;

    //cout << "energy: " << energy << endl << endl;
  return energy;

}

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
    // TODO(student): load a vertical seam
    // INFO(image);
    // INFO(start_col);
    // INFO(width);
    // INFO(height);
    // INFO(seam);

    int totalEnergy = energy(image,start_col,0,width,height);
    int energyLook[3] = {2147483647};
    int returnColVal = 0;
        

    int j = start_col;
    //cout << "J: " << j << endl;
    seam[0] = j;

    for(int i = 0; i < height-1; i++){
     
        //reset energyLook values for each loop
        energyLook[0] = 2147483647; // left
        energyLook[1] = 2147483647; // middle
        energyLook[2] = 2147483647; // right 
         
        if(j == 0){
          energyLook[1] = energy(image,j,i+1,width,height); // down
          energyLook[2] = energy(image,j+1,i+1,width,height); // down right

        }else if(j == width - 1){
          energyLook[0] = energy(image,j-1,i+1,width,height); //down left
          energyLook[1] = energy(image,j,i+1,width,height); //down

        }else{
          energyLook[0] = energy(image,j-1,i+1,width,height); //down left
          energyLook[1] = energy(image,j,i+1,width,height); //down
          energyLook[2] = energy(image,j+1,i+1,width,height); //down right     
        }

       
        int tempthing = findMin(energyLook,3,returnColVal);

        totalEnergy += tempthing;
        // cout << endl << "CURRENT POSITION: ";
        // cout << " (" << i << "," << j << ")" << endl;

        // cout << "POTENTIAL ENERGIES: " << endl;
        // cout << energyLook[0] << endl;
        // cout << energyLook[1] << endl;
        // cout << energyLook[2] << endl;

        // cout << "CHOSEN ENERGY: " << tempthing;
        // cout << " (" << i+1 << "," << j + returnColVal  << ")" << endl;

        //cout << "colval: " << returnColVal << endl;
        seam[i+1] = returnColVal + j;
        j = j + returnColVal;
        returnColVal = 0;
    }


  //cout << totalEnergy << endl;
  return totalEnergy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
    // TODO(student): load a horizontal seam
    INFO(image);
    INFO(start_row);
    INFO(width);
    INFO(height);
    INFO(seam);
  return 0;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min vertical seam
    // INFO(image);
    // INFO(width);
    // INFO(height);

    
    int* seamMin = new int[height]; //hehe
    
    int minEnergy = 2147483647;
    int currentSeamEnergy;

    for(int j = 0; j < width; j++){
      
      int* currentSeam = createSeam(height);
      currentSeamEnergy = loadVerticalSeam(image,j,width,height,currentSeam);
      
      if(currentSeamEnergy < minEnergy){
        delete [] seamMin;
        minEnergy = currentSeamEnergy;
        seamMin = currentSeam;
      }else{
        delete [] currentSeam;
      }
    }
  return seamMin;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min horizontal seam
    INFO(image);
    INFO(width);
    INFO(height);
  return nullptr;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
    // TODO(student): remove a vertical seam
    // INFO(image);
    // INFO(width);
    // INFO(height);
    // INFO(verticalSeam);

    for(int i = 0; i < height; i++){
      for(int j = verticalSeam[i]; j < width-1; j++){
        image[j][i].r = image[j+1][i].r;
        image[j][i].g = image[j+1][i].g;
        image[j][i].b = image[j+1][i].b;
      }
    }
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
    // TODO(student): remove a horizontal seam
    INFO(image);
    INFO(width);
    INFO(height);
    INFO(horizontalSeam);
}

/*
int main(){


  cout << "testing energy" << endl;

  Pixel** image = createImage(6,5);
  loadImage("test.ppm",image,6,5);

  loadVerticalSeam(image,3,6,5,createSeam(5));


  // for(int row = 0; row < 3; row++){
  //   for(int col = 0; col < 4; col++){
  //     findMinVerticalSeam(image,3,4);
  //   }
  // }

  return 0;
}
*/

int findMin(int fortnite[], int length, int &colval){

  int min = 2147483647;

  for(int i = 0; i < length; i++){

    if(fortnite[i] < min){
      min = fortnite[i];
      colval = i - 1;
    }
  }

  //if same prefer to go middle if not then increase col by 1

  if(fortnite[0] == fortnite[1] && (min == fortnite[0] || min == fortnite[1])){ //choose middles
    colval = 0;
    min = fortnite[1];

  }else if(fortnite[1] == fortnite[2] && (min == fortnite[1] || min == fortnite[2])){ //choose middle
    colval = 0;
    min = fortnite[1];

  }else if(fortnite[0] == fortnite[2] && (min == fortnite[0] || min == fortnite[2])){ //choose right
    colval = 1; // prefer to go right
    min = fortnite[2];
  }

 //cout << "MIN OF LOOK:" << min << endl;

  return min;
}
