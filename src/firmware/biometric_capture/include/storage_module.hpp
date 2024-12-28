#ifndef STORAGE_MODULE
#define STORAGE_MODULE

// Class to use SD module
class StorageModule {
  private:
    const char* queueFile = "/queue.txt";
    MQTT *mqtt;
    PubSubClient *client;

  public:
    StorageModule(MQTT *mqtt): mqtt(mqtt) {
      client = mqtt->getClient();
    }

    // Initializes the SD module
    bool begin(int chipSelectPin) {
      SPI.begin(18, 19, 23, 5);
      if (!SD.begin(5, SPI, 1000000)) {  // (Replace "5" with the correct CS pin of your module)
        Serial.println("Falha ao inicializar o SD");
        return false;
      }
      return true;
    }

    // Saves a message in the queue file
    void saveToQueue(const char* topic, const char* message) {
      File file = SD.open(queueFile, FILE_APPEND);
      if (file) {
        file.println(String(topic) + ";" + String(message));
        file.close();
        Serial.println("Mensagem salva na fila.");
      } else {
        Serial.println("Erro ao salvar na fila.");
      }
    }

    // Processes the quere messages and sends them to the broker
    void processQueue() {
      File file = SD.open(queueFile, FILE_READ);
      if (!file) {
        return; // Nenhuma mensagem na fila
      }

      String line;
      while (file.available()) {
        line = file.readStringUntil('\n');
        int separator = line.indexOf(';');
        String topic = line.substring(0, separator);
        String message = line.substring(separator + 1);
        

        if (client->connected() && mqtt->publish(topic.c_str(), message.c_str())) {
          Serial.println("Mensagem da fila enviada com sucesso.");
        } else {
          Serial.println("Falha ao enviar mensagem da fila.");
          break;
        }
      }
      file.close();

      // Rewrite the queue to remove sent messages
      rewriteQueue();
    }

  private:
    // Rewrite the queue file to remove sent messages
    void rewriteQueue() {
      File tempFile = SD.open("/temp.txt", FILE_WRITE);
      File originalFile = SD.open(queueFile, FILE_READ);

      if (originalFile && tempFile) {
        String line;
        while (originalFile.available()) {
          line = originalFile.readStringUntil('\n');
          int separator = line.indexOf(';');
          String topic = line.substring(0, separator);
          String message = line.substring(separator + 1);

          if (!client->connected() || !client->publish(topic.c_str(), message.c_str())) {
            tempFile.println(line); // Reinsere as mensagens não enviadas
          }
        }
      }

      originalFile.close();
      tempFile.close();

      SD.remove(queueFile);
      SD.rename("/temp.txt", queueFile);
    }
};

#endif