#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

int SIDE = 256;
int PERIODOS = 8;

int main(int argc, char** argv) {
  std::stringstream ss_img, ss_yml;
  cv::Mat image;
  cv::Mat graphic;

  graphic = cv::Mat::zeros(SIDE,SIDE,CV_32FC1);

  ss_yml << "senoide-" << SIDE << ".yml";
  image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);

  cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);

  for (int i = 0; i < SIDE; i++) {
    for (int j = 0; j < SIDE; j++) {
      image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS *j / SIDE) + 128;
    }
  }

  fs << "mat" << image;
  fs.release();

  cv::Mat image_yml = image.clone();

  cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
  image.convertTo(image, CV_8U);
  ss_img << "senoide-" << SIDE << ".png";
  cv::imwrite(ss_img.str(), image);



  fs.open(ss_yml.str(), cv::FileStorage::READ);
  fs["mat"] >> image;

  cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
  image.convertTo(image, CV_8U);

  int line = 5;
  
  for (int j = 0; j<SIDE ; j++){

        
        float linha = 1000*(abs(image.at<uchar>(line,j) - image_yml.at<float>(line,j)));
        graphic.at<float>(linha, j) = linha;

  }



  cv::imshow("Gráfico da imagem da senoide bidimensional", image);
  
  cv::imshow("Gráfico do erro em função da coluna",graphic);
  cv::waitKey();

  return 0;
}

