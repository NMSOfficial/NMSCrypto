#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// ASCII sanatı ile başlık yazdırma
void printAsciiArt() {
    std::cout << R"(

███╗   ██╗███╗   ███╗███████╗ ██████╗██████╗ ██╗   ██╗██████╗ ████████╗ ██████╗ 
████╗  ██║████╗ ████║██╔════╝██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝██╔═══██╗
██╔██╗ ██║██╔████╔██║███████╗██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   ██║   ██║
██║╚██╗██║██║╚██╔╝██║╚════██║██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   ██║   ██║
██║ ╚████║██║ ╚═╝ ██║███████║╚██████╗██║  ██║   ██║   ██║        ██║   ╚██████╔╝
╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝    ╚═════╝ 
                                                                            
                               
)";
}

// Kriptografi anahtarı oluşturmak için basit bir rastgele sayı üretici
std::string generateEncryptionKey() {
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const int keyLength = 16;  // Anahtar uzunluğu
    std::string key;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < keyLength; ++i) {
        key += charset[rand() % charset.length()];
    }

    return key;
}

// Şifreleme fonksiyonu (basit XOR yöntemi kullanıldı)
std::string encrypt(const std::string& message, const std::string& key) {
    std::string encryptedMessage = message;
    int keyIndex = 0;

    for (int i = 0; i < message.length(); ++i) {
        // Karakterin ASCII kodunu alıp 32 ile modunu alarak terminalde gösterilemeyecek karakterleri engelliyoruz
        encryptedMessage[i] ^= key[keyIndex] % 32;
        keyIndex = (keyIndex + 1) % key.length();
    }

    return encryptedMessage;
}

// Çözme fonksiyonu (şifreleme işlemi tersine çevrilerek)
std::string decrypt(const std::string& encryptedMessage, const std::string& key) {
    return encrypt(encryptedMessage, key);  // XOR şifreleme işlemi tersine çevrildiği için aynı fonksiyon kullanılır
}

// Metin şifreleme işlemi
void encryptText() {
    std::string message;
    std::cout << "Metni girin: ";
    std::getline(std::cin, message);

    std::string key = generateEncryptionKey();

    std::string encryptedMessage = encrypt(message, key);

    std::cout << "Şifreli metin: " << encryptedMessage << std::endl;
    std::cout << "Şifre anahtarı: " << key << std::endl;  // Şifre anahtarı direkt olarak gösteriliyor
}

// Metin çözme işlemi
void decryptText() {
    std::string encryptedMessage;
    std::string key;

    std::cout << "Şifreli metni girin: ";
    std::getline(std::cin, encryptedMessage);

    std::cout << "Varsayılan şifre anahtarınız var mı? (E/H): ";
    char choice;
    std::cin >> choice;

    if (choice == 'E' || choice == 'e') {
        // Varsayılan şifre anahtarınızı buraya girin
        key = "your_default_key_here";
    } else {
        std::cout << "Şifre anahtarını girin: ";
        std::cin.ignore();  // \n karakterini temizle
        std::getline(std::cin, key);
    }

    std::string decryptedMessage = decrypt(encryptedMessage, key);

    std::cout << "Çözülen metin: " << decryptedMessage << std::endl;
}

// Dosya şifreleme işlemi
void encryptFile() {
    std::string filename;
    std::cout << "Dosya adını girin: ";
    std::getline(std::cin, filename);

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Dosya açılamadı: " << filename << std::endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
    inputFile.close();

    std::string key = generateEncryptionKey();
    std::string encryptedContent = encrypt(content, key);

    std::ofstream outputFile(filename + ".encrypted");
    outputFile << encryptedContent;
    outputFile.close();

    std::cout << "Dosya başarıyla şifrelendi." << std::endl;
    std::cout << "Şifre anahtarı: " << key << std::endl;  // Şifre anahtarı direkt olarak gösteriliyor
}

// Dosya çözme işlemi
void decryptFile() {
    std::string filename;
    std::cout << "Şifreli dosya adını girin: ";
    std::getline(std::cin, filename);

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Dosya açılamadı: " << filename << std::endl;
        return;
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
    inputFile.close();

    std::string key;

    std::cout << "Varsayılan şifre anahtarınız var mı? (E/H): ";
    char choice;
    std::cin >> choice;

    if (choice == 'E' || choice == 'e') {
        // Varsayılan şifre anahtarınızı buraya girin
        key = "your_default_key_here";
    } else {
        std::cout << "Şifre anahtarını girin: ";
        std::cin.ignore();  // \n karakterini temizle
        std::getline(std::cin, key);
    }

    std::string decryptedContent = decrypt(encryptedContent, key);

    std::ofstream outputFile(filename.substr(0, filename.find_last_of('.')));  // .encrypted uzantısını kaldır
    outputFile << decryptedContent;
    outputFile.close();

    std::cout << "Dosya başarıyla çözüldü." << std::endl;
}

// Varsayılan şifre anahtarını ayarla
void setDefaultEncryptionKey() {
    std::string key = generateEncryptionKey();

    std::cout << "Varsayılan şifre anahtarı başarıyla ayarlandı: " << key << std::endl;
}

int main() {
    printAsciiArt();

    int choice;
    do {
        std::cout << "\nSeçenekler:\n";
        std::cout << "1) Metin Şifrele\n";
        std::cout << "2) Dosya Şifrele\n";
        std::cout << "3) Metin Çöz\n";
        std::cout << "4) Dosya Çöz\n";
        std::cout << "5) Varsayılan Şifre Anahtarı Ayarla\n";
        std::cout << "0) Çıkış\n";
        std::cout << "Seçiminizi yapın: ";
        std::cin >> choice;
        std::cin.ignore();  // \n karakterini temizle

        switch (choice) {
            case 1:
                encryptText();
                break;
            case 2:
                encryptFile();
                break;
            case 3:
                decryptText();
                break;
            case 4:
                decryptFile();
                break;
            case 5:
                setDefaultEncryptionKey();
                break;
            case 0:
                std::cout << "Programdan çıkılıyor...\n";
                break;
            default:
                std::cout << "Geçersiz seçenek! Tekrar deneyin.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

