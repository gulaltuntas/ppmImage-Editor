#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;


class ppmImage{  

    private: // private datas
    int width; 
    int height;
    int max_val; 
    vector <int> pixels;  

    public:  
    ppmImage(){  // default constructor
        width = 0;
        height = 0;
        max_val = 0;
    };

    ppmImage(int a, int b){  // constructor for dimension
    width = a; 
    height = b;
    max_val = 255;
    pixels.resize(a * b * 3);
    }
    
    ppmImage (const string &filename){  // constructor for taking the file and saving it to the object
        
         ifstream file(filename);
        if (!file.is_open()) { 
        cout << "There is not a file like that." << endl; 
         }

    string magic_number;
    file >> magic_number;
    file >> width >> height >> max_val;
 
    pixels.resize(width * height * 3); // i sized the pixels vector here

    int value; // this is for the reading every number 
     for (int i = 0; i < width * height * 3; i++) {  // since every pixel has red, green, blue values, we multiply it with three
        file >> value;
        pixels[i] = value;
    }

    file.close();
    }

    //setters & getters & functions defined

    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    void setMaxVal(int maxval) { max_val = maxval; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getMaxVal() const {   return max_val; }
    int save_ppm_image( string filename) const; // saving the image
    int read_ppm_image(string filename); // reading the image data from the file
    void print_dimension(); // dimensions
    int change_pixels(int choice);  // this is for changing the pixel places according to the input
    int  ind_pixel_info(int i, int j, char color); 


    ppmImage operator+(const ppmImage& other) const { // ++ OPERATOR OVERLOADING
    if (width != other.width || height != other.height || max_val != other.max_val) {
        cout << "Images have different dimensions or max values" << endl;
    }

    ppmImage result(width, height);
    for (size_t i = 0; i < pixels.size(); i++) {
        int val = pixels[i] + other.pixels[i];  // adding the values
        result.pixels[i] = static_cast<int>(min(val, max_val));
    }
    return result;
    }


    ppmImage operator-(const ppmImage& other) const { // - OPERATOR

    if (width != other.width || height != other.height || max_val != other.max_val) {
       cout << "Images have different dimensions or max values" << endl;
    }
    ppmImage result(width, height);
    for (size_t i = 0; i < pixels.size(); i++) {
        int val = pixels[i] - other.pixels[i];
        result.pixels[i] = static_cast<int>(max(val, 0));
    }
    return result;
    }

    ppmImage& operator=(const ppmImage & other){  // this for copying the datas
       if(this == &other) {
            return *this; // if they are already has the same values
        }
        width = other.width;
        height = other.height;
        max_val = other.max_val;
        pixels.resize(width * height * 3); // resizing our pixel size incase
       
      for (size_t i = 0; i < pixels.size(); i++) {
        pixels[i] = other.pixels[i];  // copying the values
         }
    
    return *this;
    
    
    }


    friend ostream& operator<<(ostream& os, const ppmImage& img);  // created a friend function to << 

    int& operator()(int row, int col, int color) { // () OPERATOR THAT RETURNS THE WANTED INDEX

        if (row < 0 || row >= height || col < 0 || col >= width || color < 1 || color > 3) { // if the nums are not valid
            cout << "Invalid row, column or color channel index" << endl;
        }
        int index = ((row * width + col) * 3 ) + color - 1; // calculating the which index they wanted
        return pixels[index]; // returning the reference of it 
    }
};


    ostream& operator<<(ostream& os, const ppmImage& img){ // for printing
        os << "Width: " << img.width << endl;
        os << "Height: " << img.height << endl;
        os << "Maximum pixel value: " << img.max_val << endl;
        os << "Pixels: " << endl;
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {  
            int pixel_index = (i * img.width + j) * 3;  
            os <<  img.pixels[pixel_index] << " " << img.pixels[pixel_index + 1] << " " << img.pixels[pixel_index + 2] << "  ";  
        }
        os << endl;  // a new line for every rows
    }
        return os;
    }


int ppmImage::save_ppm_image( string file_name) const{ // saving the file

 
    ofstream output_file(file_name); 
    output_file << "P3" << endl;
    output_file << width << " " << height << endl;
    output_file << "255" << endl;
        
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
        int pixel_index = (i * width + j) * 3;
        output_file << pixels[pixel_index] << " ";
        output_file << pixels[pixel_index + 1] << " ";
        output_file << pixels[pixel_index + 2] << " ";
            }
            output_file << endl;
    }
    output_file.close();
        return 1;
}

int ppmImage::read_ppm_image(string filename){  // reading the datas from the file

    ifstream file(filename);
    string magic_number;
    file >> magic_number;
    file >> width >> height >> max_val;
    pixels.resize(width * height * 3); 
    int value;
     for (int i = 0; i < width * height * 3; i++) {  
        file >> value;
        pixels[i] = value; // i took them as a flat array and arranged them later when i need to print etc
    }
    file.close();
    return 1;
}


void ppmImage::print_dimension(){
    cout << endl << width  << " x " << height;
}

int ppmImage::ind_pixel_info(int row, int col, char color){  // indivuadial pixel information
    
     if (row < 0 || row >= height || col < 0 || col >= width || color < 1 || color > 3) {
            cout << "Invalid row, column or color channel index" << endl;
        }
        int index = ((row * width + col) * 3 ) + color - 1;
        return pixels[index];
}

int ppmImage::change_pixels(int choice){  // the class function to change the pixels

   if (choice == 1){
         for(int i = 0; i < height; i++){
             for (int j = 0; j < width; j++){
                int temp = (*this)(i,j,1);  // temp to store red channel values
                (*this)(i,j,1) = (*this)(i,j,2); // swapping with green
                 (*this)(i,j,2) = temp;
              }
            }
    } 

   if (choice == 2){
       for(int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
             int temp = (*this)(i,j,1); // temp to store the red channel values
             (*this)(i,j,1) = (*this)(i,j,3); // swapping with blue
             (*this)(i,j,3) = temp;
                 }
             }
    }

   if (choice == 3){
         for(int i = 0; i < height; i++){
             for (int j = 0; j < width; j++){
                int temp = (*this)(i,j,2); // temp to store green channel values
                 (*this)(i,j,2) = (*this)(i,j,3); // swapping with blue
                 (*this)(i,j,3) = temp; 
                }
            }
    }
    return 1;
}

// STANDALON FUNCTIONS

int read_ppm(const string source_ppm_file_name, ppmImage& destination_object);
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object);
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3);
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3);
int test_print(const string filename_image1);
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice);
ppmImage single_color(const ppmImage& source, int color_choice);



int main(int argc, char** argv){


    if (argc > 1){

        int number;	// number choice
		string ppmFileName1(argv[2]), ppmFileName2(argv[3]);	// file names

		number = stoi(argv[1]);	// turning argument number choice to an integer 
        ppmImage readPPMImage;		// image of read file
		ppmImage processedPPMImage;	// result image

        	switch(number){
			case 1:
				if(argc != 5) return 0;
				test_addition(ppmFileName1, ppmFileName2, argv[4]);
				break;
			case 2:
				if(argc != 5) return 0;
				test_subtraction(ppmFileName1, ppmFileName2, argv[4]);
				break;
			case 3:
				read_ppm(ppmFileName1, readPPMImage);
				swap_channels(readPPMImage, 2);
				write_ppm(ppmFileName2, readPPMImage);
				break;
			case 4:
				read_ppm(ppmFileName1, readPPMImage);
				swap_channels(readPPMImage, 3);
				write_ppm(ppmFileName2, readPPMImage);
				break;
			case 5:
				read_ppm(ppmFileName1, readPPMImage);
				processedPPMImage = single_color(readPPMImage, 1);
				write_ppm(ppmFileName2, processedPPMImage);
				break;
			case 6:
				read_ppm(ppmFileName1, readPPMImage);
				processedPPMImage = single_color(readPPMImage, 2);
				write_ppm(ppmFileName2, processedPPMImage);
				break;
			case 7:
				read_ppm(ppmFileName1, readPPMImage);
				processedPPMImage = single_color(readPPMImage, 3);
				write_ppm(ppmFileName2, processedPPMImage);
				break;
            case 8:
                test_print(ppmFileName1);
                break;
			default:
				cout << "Invalid choice number!" << endl;
		}

    }

    return 0;
}



int read_ppm(const string source_ppm_file_name, ppmImage& destination_object){
   return destination_object.read_ppm_image(source_ppm_file_name);	
}

int write_ppm(const string destination_ppm_file_name, const ppmImage &source_object){
	return source_object.save_ppm_image(destination_ppm_file_name);	
}

int swap_channels(ppmImage &image_object_to_be_modified, int swap_choice){
	return image_object_to_be_modified.change_pixels(swap_choice);	
}

int test_addition(const string filename_image1, const string filename_image2, const string filename_image3){
	ppmImage image1(filename_image1), image2(filename_image2);	// initializing the images
	ppmImage sumImage = image1 + image2;						// adding them
    return write_ppm(filename_image3, sumImage);	// writing sum
    }


int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3){
	ppmImage image1(filename_image1), image2(filename_image2);	// initializing the images
	ppmImage sumImage = image1 - image2;						// substract
	return write_ppm(filename_image3, sumImage);				// write sum
			
}

int test_print(const string filename_image1){
	ppmImage image(filename_image1);	
	cout << image;					// printing the image <<
	return 1;
}


ppmImage single_color(const ppmImage& source, int color_choice){

    ppmImage image = source; // using operator= we copied the source image to our image 

    if (color_choice == 1){
            for(int i = 0; i < image.getHeight(); i++){  // height loop
             for (int j = 0; j < image.getWidth(); j++){ // width loop
                image(i,j,2) = 0; // change the green values to 0
                image(i,j,3) = 0; // change the blue values to 0
              }
            }
        
    }
    if (color_choice == 2){
        for(int i = 0; i < image.getHeight(); i++){
             for (int j = 0; j < image.getWidth(); j++){
                image(i,j,1) = 0; // change the red values to 0
                image(i,j,3)= 0; // change the blue values to 0
              }
            }
    }
    if (color_choice == 3){
        for(int i = 0; i < image.getHeight(); i++){
             for (int j = 0; j < image.getWidth(); j++){
                image(i,j,1) = 0; // change the red values to 0
                image(i,j,2) = 0; // change the green values to 0
              }
            }
    }

   return image; // returning image
}