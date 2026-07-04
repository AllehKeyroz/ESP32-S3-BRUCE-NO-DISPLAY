# Plano: Bruce com Display Virtual (HAS_SCREEN=1)

## Objetivo
Recompilar o Bruce com suporte a `HAS_SCREEN` ativado, mas sem display físico. O código de renderização escreverá em um **buffer de memória** (framebuffer) que o WebUI Navigator pode ler e exibir no navegador, dando a ilusão de um display real.

## Estratégia

### 1. Modificar `boards/N16R8/pins_arduino.h`
Adicionar:
```c
#define HAS_SCREEN 1
#define TFT_WIDTH 320
#define TFT_HEIGHT 240
```

### 2. Criar um "display virtual" em `boards/N16R8/interface.cpp`
Em vez de inicializar um display TFT real (SPI, pinos), criar uma implementação que:
- Aloca um buffer de memória (320x240x2 = 150KB) na PSRAM
- Implementa as funções TFT_eSPI usando o buffer
- O Navigator do WebUI lê esse buffer e mostra no navegador

### 3. Implementar funções TFT_eSPI stub
As funções que o Bruce chama para desenhar na tela precisam ser implementadas:
- `fillScreen`, `fillRect`, `drawPixel`, `setCursor`, `print`, `drawCentreString`
- `setTextColor`, `setTextSize`, `setRotation`
- `drawRect`, `fillRoundRect`, `drawRoundRect`
- `drawFastHLine`, `drawFastVLine`
- `createSprite`, `pushSprite`, `deleteSprite`
- `width`, `height`, `getRotation`

### 4. Risco: TFT_eSPI library
O código do Bruce usa TFT_eSPI extensivamente. Mesmo com `HAS_SCREEN`, ele inclui `<TFT_eSPI.h>` que precisa estar disponível. Duas opções:
- **A:** Instalar a TFT_eSPI library real (ela compila mesmo sem display, só precisa de `User_Setup.h`)
- **B:** Criar um stub mínimo de TFT_eSPI que simula as funções

**Opção recomendada: A** — Instalar TFT_eSPI com um `User_Setup.h` custom que define display virtual.

### 5. Arquivo `User_Setup.h` para display virtual
```c
#define ILI9341_DRIVER
#define TFT_WIDTH 320
#define TFT_HEIGHT 240
#define TFT_CS -1
#define TFT_DC -1
#define TFT_RST -1
#define TFT_BL -1
#define SPI_FREQUENCY 1000000
#define SPI_READ_FREQUENCY 1000000
```

### 6. Riscos e Contingências

| Risco | Probabilidade | Mitigação |
|-------|--------------|-----------|
| TFT_eSPI tenta acessar pinos -1 (CS, DC) | Alta | Modificar `interface.cpp` para anular pinos |
| Consumo de RAM (framebuffer 150KB) | Médio | Usar PSRAM (8GB disponível) |
| Lentidão no Navigator | Médio | Comprimir framebuffer antes de enviar |
| Dependências de display no código Bruce | Baixa | O código Bruce usa `tft` object global |

### 7. Funções que ficarão disponíveis COM display

#### Tela Inicial
- Banner "PREDATORY FIRMWARE" com tubarão 🦈 ASCII
- Relógio
- Status WiFi/BT/Bateria

#### Menu Principal
- **WiFi** → Scan APs, Scan Hosts, Beacon Spam, Deauth, Evil Portal, Sniffer, Karma, Wardriving
- **BLE** → Scan, Spam (iOS/Android/Samsung/Windows/All), Beacon Spam
- **RF** → Scan, Rx, Tx, Spectrum, Jammer, Brute Force
- **RFID** → Read, Write, Clone, Emulate, Amiibo, PN532
- **IR** → TV-B-Gone, Read, Send, Custom Protocols
- **NRF24** → Jammer, Spectrum, Mousejack
- **Scripts** → JavaScript Interpreter
- **GPS** → Wardriving, GPS Tracker
- **Others** → BadUSB, iButton, QR Code, Mic Spectrum, Clicker, Timer
- **Config** → Brightness, Dim Time, Orientation, UI Color, Boot Sound, Sleep, Clock, WiFi At Startup
- **Files** → File Manager (SD/LittleFS), WebUI, Navigator
- **Connect** → ESP-NOW Send/Receive (File/Commands)
- **Clock** → RTC, NTP, Manual

#### WebUI completo (já existe mas com Navigator funcional)
- Navegação visual pelos menus usando Navigator
- Editor de arquivos
- Serial Cmd
- Settings
- Upload/Download de arquivos

### 8. Fluxo de implementação

```
1. pins_arduino.h → add HAS_SCREEN
2. Criar User_Setup.h para TFT_eSPI (pinos -1)
3. Instalar TFT_eSPI no PlatformIO
4. interface.cpp → stub TFT virtual com buffer PSRAM
5. Compilar e testar
6. Se crash: depurar (provavelmente pinMode com pinos -1)
7. Ajustar até funcionar
```

### 9. Tempo estimado
- Configuração: 15min
- Implementação do stub: 30min
- Compilação: 10min
- Debug: variável (30min-2h)
- **Total: ~1-3h**
