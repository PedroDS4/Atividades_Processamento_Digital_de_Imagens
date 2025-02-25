#include <iostream>
#include <opencv2/opencv.hpp>


class point
{

private:
  double x;
  double y;
  
public:
  point(double x_1, double y_1);
  point();

  // Métodos para acessar os valores
  double getX() const;
  double getY() const;

  
};


point::point(double x_1, double y_1): x(x_1), y(y_1){}
point::point(): x(0.0), y(0.0){}

double point::getX() const{
    return x;
}

double point::getY() const{
    return y;
}


cv::Mat negative(point P1, point P2, cv::Mat imagem){
    double x1;
    double x2;
    double y1;
    double y2;
    cv::Mat image = imagem.clone();
    if (P1.getX()>P2.getX()){
        x1 = P2.getX();
        x2 = P1.getX();
        if(P1.getY()>P2.getY()){
            y1 = P2.getY();
            y2 = P1.getY();
        }
        else{
            y1 = P1.getY();
            y2 = P2.getY();
        }
    }
    else{
        x1 = P1.getX();
        x2 = P2.getX(); 
        if(P1.getY()>P2.getY()){
            y1 = P2.getY();
            y2 = P1.getY();
        }
        else{
            y1 = P1.getY();
            y2 = P2.getY();
        }
    }

    for (int i = x1; i <x2 ; i++) {
        for (int j = y1 ; j < y2; j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i,j) ;

        }
    }

    return image;


}




int main(int, char**) {
  cv::Mat image;
  cv::Vec3b val;

  //image = cv::imread("biel.png", cv::IMREAD_COLOR);
  image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);
  if (!image.data) std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_NORMAL);
  
  double x1;
  double y1;
  double x2;
  double y2;

  std::cout<<"Digite as coordenadas do ponto P1"<<std::endl;
  std::cin>>x1>>y1;
  std::cout<<"Digite as coordenadas do ponto P2"<<std::endl;
  std::cin>>x2>>y2;

  point P1(x1,y1);
  point P2(x2,y2); 
  cv::Mat imagem = negative(P1,P2,image);

  cv::imshow("janela", imagem);
  cv::waitKey();
  return 0;
}
