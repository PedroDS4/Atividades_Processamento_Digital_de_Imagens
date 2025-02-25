#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image, image_32F, imagem_borrada;

    // Carrega a imagem em escala de cinza
    image = cv::imread("image.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }

    image.convertTo(image_32F, CV_32F);
    int N = 3;  // Valor inicial da máscara (3x3)

    while (true) {
        int meio = (N - 1) / 2;

        // Cria uma máscara NxN com valores iguais e aplica o filtro
        cv::Mat mask = cv::Mat::ones(N, N, CV_32F) / (float)(N * N);
        cv::filter2D(image_32F, imagem_borrada, image_32F.depth(), mask, cv::Point(meio, meio), cv::BORDER_REPLICATE);

        // Converte para 8 bits para exibição
        cv::Mat imagem_borrada_8U;
        imagem_borrada.convertTo(imagem_borrada_8U, CV_8U);

        // Exibe o resultado
        cv::imshow("Imagem Borrada", imagem_borrada_8U);

        // Exibe o valor atual de N
        std::cout << "Dimensão da máscara: " << N << "x" << N << std::endl;

        // Espera por uma tecla
        int key = cv::waitKey();

        // Aumenta o tamanho da máscara ao pressionar a tecla "Seta para cima" e decrementa com "Seta para baixo"
        if (key == 27) // ESC para sair
            break;
        else if (key == 82) // Tecla "Seta para cima" (código 82)
            N += 2; // Aumenta N para o próximo ímpar
        else if (key == 84 && N > 3) // Tecla "Seta para baixo" (código 84)
            N -= 2; // Diminui N para o ímpar anterior

        // Limpa a janela
        cv::destroyWindow("Imagem Borrada");
    }

    return 0;
}

