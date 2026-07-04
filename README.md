# ESP32-S3 Bruce - No Display (Headless)

Firmware **Bruce** compilado para **ESP32-S3 sem display** (headless). Controle total via:

- **WebUI** — painel web acessível pelo navegador (Serial Cmd, File Manager)
- **BLE Serial** — aplicativo Bluetooth Serial no celular
- **USB Serial** — terminal serial direto (115200 baud)

Compilado para a placa **ESP32-S3 N16R8** (16MB Flash + 8MB PSRAM Octal).

> ⚠️ **Headless:** Esta versão não possui interface de display/touch. O Navigator do WebUI não exibe menus gráficos. Use o **Serial Cmd** no WebUI para executar comandos.

## Especificações

| Item | Detalhe |
|------|---------|
| **Chip** | ESP32-S3 (Xtensa LX7 dual-core 240MHz) |
| **Flash** | 16MB |
| **PSRAM** | 8MB OPI (Octal) |
| **WiFi** | 2.4GHz b/g/n |
| **Bluetooth** | BLE 5.0 |
| **USB** | Nativo (CDC serial) + UART externo (CH343/CH9102) |

## Instalação

### Pré-requisitos

- Python 3 + esptool (`pip install esptool`)
- ESP32-S3 N16R8 conectado via USB

### Flash rápido (binário pré-compilado)

```bash
# Identifique a porta COM (Windows) ou /dev/tty (Linux/Mac)
esptool --chip esp32s3 --port COM4 --baud 921600 erase_flash

esptool --chip esp32s3 --port COM4 --baud 921600 write_flash -z 0x0 firmware/bruce-esp32-s3-n16r8-no-display.bin
```

**Modo de download:** Segure BOOT, pressione RESET, solte BOOT.

### Compilação manual (PlatformIO)

```bash
# Instalar PlatformIO
pip install platformio

# Clonar este repositório
git clone https://github.com/AllehKeyroz/ESP32-S3-BRUCE-NO-DISPLAY
cd ESP32-S3-BRUCE-NO-DISPLAY

# Compilar (leva ~10min na primeira vez)
pio run -e esp32-s3-n16r8

# Flash
pio run -e esp32-s3-n16r8 -t upload --upload-port COM4
```

## Uso

### Acesso WebUI

O Bruce cria uma rede WiFi **BruceNet** (senha: **brucenet**).

| Passo | Ação |
|-------|------|
| 1 | Conecte-se à rede **BruceNet** |
| 2 | Abra o navegador em **http://192.168.4.1** |
| 3 | Login: **admin** / **bruce** |
| 4 | Clique em **Serial Cmd** para enviar comandos |

### Conectar em rede WiFi existente

No Serial Cmd do WebUI (ou via BLE/USB Serial):

```bash
wifi add "MinhaRede" "minha_senha"
wifi on
```

O IP será atribuído via DHCP. Descubra-o no roteador.

### Comandos disponíveis

#### WiFi
| Comando | Descrição |
|---------|-----------|
| `wifi on` | Conecta à rede salva ou cria AP |
| `wifi off` | Desconecta WiFi |
| `wifi add "SSID" "senha"` | Adiciona rede WiFi |
| `beaconspam -r` | Beacon spam aleatório |
| `deauth` | Deauth flood |
| `sniffer` | Raw sniffer |
| `arp` | ARP scan |
| `listen` | TCP listener |

#### Bluetooth
| Comando | Descrição |
|---------|-----------|
| `blespam -t all` | BLE spam (todas plataformas) |
| `blespam -t sourapple` | iOS Apple spam |
| `blespam -t samsung` | Samsung spam |
| `blespam -t windows` | Windows Swiftpair spam |
| `blespam -t google` | Android/Google spam |
| `blespam -t flipper` | Flipper Zero style |

#### IR
| Comando | Descrição |
|---------|-----------|
| `ir rx <timeout>` | Ler sinal IR |
| `ir rx raw <timeout>` | Ler IR em modo RAW |
| `ir tx <protocolo> <addr> <valor>` | Enviar sinal IR |
| `ir tx_from_file <arquivo>` | Enviar IR salvo |

#### RF (Sub-GHz)
| Comando | Descrição |
|---------|-----------|
| `subghz rx <timeout>` | Ler sinal RF |
| `subghz tx <valor> <freq> <te> <count>` | Enviar sinal RF |
| `subghz tx_from_file <arquivo>` | Enviar RF salvo |

#### Áudio
| Comando | Descrição |
|---------|-----------|
| `music_player <arquivo>` | Tocar áudio |
| `tone <freq> <duração>` | Tom sonoro |
| `say <texto>` | Text-to-speech |

#### Sistema
| Comando | Descrição |
|---------|-----------|
| `webui` | Inicia WebUI |
| `webui -noAp` | WebUI sem criar AP |
| `settings` | Mostra configs |
| `settings <nome> <valor>` | Altera config |
| `factory_reset` | Reset de fábrica |
| `power reboot` | Reinicia |
| `power off` | Desliga |
| `power sleep` | Modo sleep |
| `gpio mode <pin> <0/1>` | Configura GPIO |
| `gpio set <pin> <0/1>` | Controla GPIO |
| `i2c scan` | Escaneia barramento I2C |
| `storage list` | Lista arquivos |
| `clock` | Mostra relógio |
| `led <r> <g> <b>` | Cor do LED |

## Estrutura do Repositório

```
ESP32-S3-BRUCE-NO-DISPLAY/
├── README.md
├── platformio.ini
├── custom_16Mb.csv              # Tabela de partições
├── build.py                     # Script de build
├── boards/
│   └── N16R8/                   # Board custom para ESP32-S3
│       ├── pins_arduino.h
│       └── interface.cpp
├── firmware/
│   └── bruce-esp32-s3-n16r8-no-display.bin
├── .gitignore
└── src/                         # Código fonte (extrair do Bruce oficial)
```

## Compilação personalizada

Para alterar configurações, edite `platformio.ini` e recompile:

```ini
[env:esp32-s3-n16r8]
board = ES3C28P
board_build.arduino.memory_type = qio_opi
board_build.flash_mode = qio  
board_build.partitions = custom_16Mb.csv
board_upload.flash_size = 16MB
build_flags =
    ${env.build_flags}
    -Iboards/N16R8
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DBOARD_HAS_PSRAM
    -DCONFIG_SPIRAM_MODE_OCT=1
    -DCONFIG_ESP32S3_DATA_CACHE_LINE_SIZE=64
    -DCORE_DEBUG_LEVEL=1
    -DDISABLE_ALL_LIBRARY_WARNINGS
build_src_filter = ${env.build_src_filter} +<../boards/N16R8>
board_build.variants_dir = boards
lib_ignore = FastLED, ESP8266Audio, ESP8266SAM, Adafruit_Si4713
```

## Licença

Este firmware é baseado no [Bruce](https://github.com/BruceDevices/firmware) por BruceDevices, licenciado sob AGPL-3.0.
