#include <opencv2/opencv.hpp>
#include <iostream>

// Variáveis para sliders (trackbars)
int alpha_slider = 50; // controla o decaimento
int region_slider = 50; // controla a posição da região focal
int height_slider = 50; // controla a altura da região focal

int alpha_slider_max = 100;
int region_slider_max = 100;
int height_slider_max = 100;

cv::Mat image, blurred, tiltshift_image;

// Função para calcular a máscara de ponderação usando tangente hiperbólica
cv::Mat calculateAlphaMask(int region_center, int height, int decay) {
    int rows = image.rows;
    int cols = image.cols;
    cv::Mat alpha_mask = cv::Mat(rows, cols, CV_32F);  // Único canal

    int l1 = region_center - height / 2;
    int l2 = region_center + height / 2;

    for (int y = 0; y < rows; y++) {
        float alpha_value = 0.5 * (std::tanh((y - l1) / (float)decay) - std::tanh((y - l2) / (float)decay));
        alpha_mask.row(y).setTo(alpha_value);
    }

    // A máscara permanece com 1 canal
    return alpha_mask;
}

// Função para aplicar o efeito tilt-shift
void applyTiltShift() {
    int region_center = (region_slider * image.rows) / region_slider_max;
    int height = (height_slider * image.rows) / height_slider_max;
    int decay = (alpha_slider * 20) / alpha_slider_max + 1;

    // Calcula o mapa de ponderação
    cv::Mat alpha_mask = calculateAlphaMask(region_center, height, decay);

    // Converte a máscara alpha para 3 canais
    cv::Mat alpha, inverted_alpha;
    cv::Mat alpha_rgb;
    cv::merge(std::vector<cv::Mat>{alpha_mask, alpha_mask, alpha_mask}, alpha_rgb);

    // Inverte a máscara para a região desfocada
    inverted_alpha = 1.0 - alpha_rgb;

    // Converte imagens para o tipo float
    cv::Mat image_float, blurred_float;
    image.convertTo(image_float, CV_32FC3, 1.0 / 255.0);

    // Aplica o filtro de média para criar a imagem borrada
    cv::Mat blurred_temp;
    int kernel_size = 15; // Tamanho do kernel do filtro de média
    cv::blur(image, blurred_temp, cv::Size(kernel_size, kernel_size));

    blurred_temp.convertTo(blurred_float, CV_32FC3, 1.0 / 255.0);

    // Multiplica as imagens pela máscara
    cv::Mat original_weighted, blurred_weighted;
    cv::multiply(image_float, alpha_rgb, original_weighted);
    cv::multiply(blurred_float, inverted_alpha, blurred_weighted);

    // Combina as imagens ponderadas
    cv::add(original_weighted, blurred_weighted, tiltshift_image);
    tiltshift_image.convertTo(tiltshift_image, CV_8UC3, 255.0);

    // Exibe a imagem resultante
    cv::imshow("Tilt-Shift Effect", tiltshift_image);
}

// Funções de callback para cada slider
void onAlphaChange(int, void*) {
    applyTiltShift();
}

void onRegionChange(int, void*) {
    applyTiltShift();
}

void onHeightChange(int, void*) {
    applyTiltShift();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem>" << std::endl;
        return -1;
    }

    // Carrega a imagem de entrada
    image = cv::imread(argv[1]);
    if (image.empty()) {
        std::cerr << "Erro ao carregar a imagem." << std::endl;
        return -1;
    }

    // Aplica o filtro de média na imagem
    int kernel_size = 15; // Tamanho do kernel do filtro de média
    cv::blur(image, blurred, cv::Size(kernel_size, kernel_size));

    cv::namedWindow("Tilt-Shift Effect", cv::WINDOW_AUTOSIZE);

    // Cria os sliders para ajustar o valor de alpha, a posição da região e a altura da região
    cv::createTrackbar("Decaimento", "Tilt-Shift Effect", &alpha_slider, alpha_slider_max, onAlphaChange);
    cv::createTrackbar("Posição Região", "Tilt-Shift Effect", &region_slider, region_slider_max, onRegionChange);
    cv::createTrackbar("Altura Região", "Tilt-Shift Effect", &height_slider, height_slider_max, onHeightChange);

    // Aplica o efeito tilt-shift com os valores padrão dos sliders
    applyTiltShift();

    // Espera até que o usuário pressione uma tecla e então salva a imagem
    cv::waitKey(0);

    cv::imwrite("tiltshift_result.jpg", tiltshift_image);
    std::cout << "Imagem tilt-shift salva como 'tiltshift_result.jpg'" << std::endl;

    return 0;
}

