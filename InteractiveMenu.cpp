#include "InteractiveMenu.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

InteractiveMenu::InteractiveMenu() 
    : current_state_(State::MAIN_MENU), 
      min_match_length_(10), 
      selected_option_(0) {
    // Configurar terminal para ANSI en Windows si es necesario
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif
}

void InteractiveMenu::clear_screen() {
    std::cout << "\033[2J\033[1;1H";
}

void InteractiveMenu::print_centered(const std::string& text, const std::string& color) {
    int width = 80;
    int padding = (width - text.length()) / 2;
    if (padding < 0) padding = 0;
    std::cout << std::string(padding, ' ') << color << text << RESET << std::endl;
}

void InteractiveMenu::print_header(const std::string& title) {
    clear_screen();
    std::cout << CYAN << std::string(80, '=') << RESET << std::endl;
    print_centered("DETECTOR DE PLAGIO INTERACTIVO", BOLD + MAGENTA);
    std::cout << CYAN << std::string(80, '-') << RESET << std::endl;
    print_centered(title, YELLOW);
    std::cout << CYAN << std::string(80, '=') << RESET << std::endl << std::endl;
}

void InteractiveMenu::print_footer() {
    std::cout << std::endl << CYAN << std::string(80, '-') << RESET << std::endl;
    std::cout << " " << BOLD << "ENTER" << RESET << ": Seleccionar  " 
              << BOLD << "1-5" << RESET << ": Opciones  " 
              << BOLD << "Q" << RESET << ": Salir" << std::endl;
}

void InteractiveMenu::wait_for_key() {
    std::cout << "\nPresione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string InteractiveMenu::get_input(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void InteractiveMenu::run() {
    while (current_state_ != State::EXIT) {
        switch (current_state_) {
            case State::MAIN_MENU:
                show_main_menu();
                break;
            case State::INPUT_TEXT_1:
                handle_input_text(1);
                break;
            case State::INPUT_TEXT_2:
                handle_input_text(2);
                break;
            case State::SELECT_TEST_CASE:
                show_test_cases();
                break;
            case State::SHOW_RESULTS:
                show_results();
                break;
            case State::EXIT:
                break;
        }
    }
    clear_screen();
    std::cout << "¡Gracias por usar el Detector de Plagio!" << std::endl;
}

void InteractiveMenu::show_main_menu() {
    print_header("MENÚ PRINCIPAL");

    std::cout << "  " << (text1_.empty() ? RED : GREEN) << "1. Ingresar Texto 1 " 
              << (text1_.empty() ? "(Vacío)" : "(Listo)") << RESET << std::endl;
    std::cout << "  " << (text2_.empty() ? RED : GREEN) << "2. Ingresar Texto 2 " 
              << (text2_.empty() ? "(Vacío)" : "(Listo)") << RESET << std::endl;
    std::cout << "  " << BLUE << "3. Cargar Caso de Prueba Predefinido" << RESET << std::endl;
    std::cout << "  " << YELLOW << "4. Configurar Longitud Mínima (" << min_match_length_ << ")" << RESET << std::endl;
    
    bool can_analyze = !text1_.empty() && !text2_.empty();
    std::cout << "  " << (can_analyze ? BOLD + GREEN : BOLD + RED) << "5. INICIAR ANÁLISIS" << RESET << std::endl;
    std::cout << "  " << "6. Salir" << std::endl;

    print_footer();
    std::cout << "\nOpción: ";
    
    std::string input;
    std::getline(std::cin, input);
    
    if (input == "1") current_state_ = State::INPUT_TEXT_1;
    else if (input == "2") current_state_ = State::INPUT_TEXT_2;
    else if (input == "3") current_state_ = State::SELECT_TEST_CASE;
    else if (input == "4") {
        std::cout << "Nueva longitud mínima: ";
        std::cin >> min_match_length_;
        std::cin.ignore();
    }
    else if (input == "5") {
        if (can_analyze) run_analysis();
        else {
            std::cout << RED << "\nError: Debe ingresar ambos textos primero." << RESET << std::endl;
            wait_for_key();
        }
    }
    else if (input == "6" || input == "q" || input == "Q") current_state_ = State::EXIT;
}

void InteractiveMenu::handle_input_text(int text_num) {
    print_header("INGRESAR TEXTO " + std::to_string(text_num));
    std::cout << "Escriba o pegue el texto (presione ENTER al finalizar):" << std::endl;
    std::cout << YELLOW << "> " << RESET;
    
    std::string input;
    std::getline(std::cin, input);
    
    if (!input.empty()) {
        if (text_num == 1) text1_ = input;
        else text2_ = input;
        std::cout << GREEN << "\nTexto guardado correctamente." << RESET << std::endl;
    }
    
    current_state_ = State::MAIN_MENU;
}

void InteractiveMenu::show_test_cases() {
    print_header("SELECCIONAR CASO DE PRUEBA");
    
    std::cout << "1. Plagio Parcial Obvio" << std::endl;
    std::cout << "2. Textos Académicos Similares" << std::endl;
    std::cout << "3. Código Fuente Duplicado" << std::endl;
    std::cout << "4. Textos Completamente Diferentes" << std::endl;
    std::cout << "5. Volver" << std::endl;
    
    std::cout << "\nOpción: ";
    std::string input;
    std::getline(std::cin, input);
    
    if (input == "1") {
        text1_ = "El Suffix Array es una estructura de datos muy potente para búsquedas de patrones. Permite encontrar todas las ocurrencias de un patrón en tiempo logarítmico.";
        text2_ = "El Suffix Array es una estructura de datos muy potente para búsquedas de patrones. Es una herramienta fundamental en bioinformática.";
        min_match_length_ = 15;
    } else if (input == "2") {
        text1_ = "La complejidad temporal del algoritmo de construcción del Suffix Array utilizando el método de duplicación es O(n log n).";
        text2_ = "El método de duplicación para construir el Suffix Array tiene una complejidad temporal de O(n log n).";
        min_match_length_ = 12;
    } else if (input == "3") {
        text1_ = "for(int i=0; i<n; i++) { sum += arr[i]; }";
        text2_ = "int s=0; for(int i=0; i<n; i++) { sum += arr[i]; } return s;";
        min_match_length_ = 10;
    } else if (input == "4") {
        text1_ = "El gato come pescado.";
        text2_ = "La computadora procesa datos.";
        min_match_length_ = 5;
    }
    
    if (input != "5") {
        std::cout << GREEN << "\nCaso de prueba cargado." << RESET << std::endl;
        wait_for_key();
    }
    current_state_ = State::MAIN_MENU;
}

void InteractiveMenu::run_analysis() {
    print_header("ANALIZANDO...");
    std::cout << "Procesando textos..." << std::endl;
    
    // Simular barra de progreso
    std::cout << "[";
    for(int i=0; i<40; i++) {
        std::cout << "=";
        std::cout.flush();
        // Pequeña pausa para efecto visual (opcional, removida para velocidad)
    }
    std::cout << "] 100%" << std::endl;
    
    PlagiarismDetector detector(text1_, text2_, min_match_length_);
    last_report_ = detector.analyze();
    
    current_state_ = State::SHOW_RESULTS;
}

void InteractiveMenu::show_results() {
    print_header("RESULTADOS DEL ANÁLISIS");
    
    // Color según porcentaje
    std::string sim_color = GREEN;
    if (last_report_.similarity_percentage > 30) sim_color = YELLOW;
    if (last_report_.similarity_percentage > 70) sim_color = RED;
    
    std::cout << BOLD << "SIMILITUD TOTAL: " << sim_color 
              << std::fixed << std::setprecision(2) << last_report_.similarity_percentage << "%" 
              << RESET << std::endl << std::endl;
              
    std::cout << "Fragmentos encontrados: " << last_report_.matches.size() << std::endl;
    std::cout << "Longitud máxima común:  " << last_report_.longest_match << std::endl;
    std::cout << "Caracteres totales:     " << last_report_.total_matched_chars << std::endl;
    
    if (!last_report_.matches.empty()) {
        std::cout << "\n" << BOLD << "TOP 5 COINCIDENCIAS:" << RESET << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        auto matches = last_report_.matches;
        std::sort(matches.begin(), matches.end(), 
                 [](const auto& a, const auto& b) { return a.length > b.length; });
                 
        int count = std::min(5, (int)matches.size());
        for(int i=0; i<count; i++) {
            const auto& m = matches[i];
            std::cout << YELLOW << "[" << i+1 << "] " << RESET 
                      << "Longitud: " << m.length << " chars" << std::endl;
            std::cout << "    \"" << CYAN << m.text.substr(0, 60) 
                      << (m.text.length() > 60 ? "..." : "") << RESET << "\"" << std::endl;
        }
    } else {
        std::cout << "\n" << GREEN << "No se encontraron coincidencias sospechosas." << RESET << std::endl;
    }
    
    std::cout << "\nPresione ENTER para volver al menú...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar buffer si quedó algo
    std::cin.get(); // Esperar enter
    
    current_state_ = State::MAIN_MENU;
}
