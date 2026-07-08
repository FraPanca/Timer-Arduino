# Timer-Arduino

## Italiano

Un timer digitale controllato da telecomando a infrarossi, con display a 4 cifre e allarme sonoro.

**Stack:** C/C++ (Arduino) · Libreria IRremote 2.7.0 · Registro a scorrimento 74HC595 (display multiplexato)

### Descrizione

Realizzazione di un timer con una scheda Arduino UNO R3, controllato interamente tramite telecomando a infrarossi. È possibile impostare un tempo cifra per cifra, avviarlo, metterlo in pausa, regolare il volume dell'allarme sonoro e interromperlo una volta scattato. Il tempo (nel formato mm:ss) viene mostrato su un display a 4 cifre a 7 segmenti, pilotato tramite un registro a scorrimento 74HC595 con tecnica di multiplexing.

### Componenti necessari

- UNO R3 Controller Board
- 4 Digit 7-Segment Display
- IR Receiver Module
- Remote Control
- Passive Buzzer
- 74HC595 IC
- 5 resistori 220 Ohm
- Breadboard
- Breadboard jumpers

### Come si esegue

1. Installa la libreria **IRremote** (versione 2.7.0) nell'IDE Arduino.
2. Apri `Timer.ino`, seleziona la scheda "Arduino UNO" e la porta seriale corretta.
3. Collega i componenti secondo lo schema elettrico incluso (`Timer-Tinkercad.png` / `Timer.pdf`).
4. Carica lo sketch sulla scheda.

### Funzionalità principali

- Impostazione del tempo cifra per cifra tramite telecomando IR, con selezione della posizione (up/down) e inserimento dei valori (tasti numerici 0-9); la cifra delle decine di secondi è vincolata a un massimo di 5, coerentemente con il formato mm:ss
- Avvio/pausa del conto alla rovescia e accensione/spegnimento generale del dispositivo tramite telecomando
- Allarme sonoro (buzzer passivo) allo scadere del timer, con volume regolabile tramite telecomando (agisce sulla frequenza del segnale generato)
- Display multiplexato a 4 cifre a 7 segmenti pilotato tramite registro a scorrimento 74HC595, con lampeggio della cifra in fase di modifica durante l'impostazione del tempo
- Decodifica dei codici IR ricevuti tramite libreria IRremote, con mappatura dei tasti documentata in `decodifica_tasti.txt`
- Log diagnostico su Serial Monitor (9600 baud) per il debug dei codici IR ricevuti e delle modifiche al tempo impostato

### Struttura del progetto

```
Timer-Arduino/
├── Timer.ino                 # Sketch principale: gestione IR, display multiplexato, buzzer, logica del timer
├── decodifica_tasti.txt      # Mappatura dei codici IR del telecomando ai rispettivi tasti
├── Timer-Tinkercad.png       # Schema elettrico del circuito
└── Timer.pdf                 # Schema elettrico (esportazione Tinkercad)
```

### Note

Progetto didattico/personale con Arduino UNO R3; richiede la libreria IRremote in versione 2.7.0 (versioni più recenti hanno un'API differente per la decodifica dei segnali). I codici del telecomando sono cablati nel codice sorgente e sono specifici del modello di telecomando utilizzato (documentati in `decodifica_tasti.txt`): con un telecomando diverso vanno ridefiniti.

### Licenza

MIT

---

## English

A digital timer controlled by an infrared remote, with a 4-digit display and a sound alarm.

**Stack:** C/C++ (Arduino) · IRremote library 2.7.0 · 74HC595 shift register (multiplexed display)

### Description

Implementation of a timer using an Arduino UNO R3 board, fully controlled via an infrared remote control. The time can be set digit by digit, started, paused, and the alarm volume can be adjusted; the timer can be stopped once it goes off. The time (in mm:ss format) is shown on a 4-digit 7-segment display, driven through a 74HC595 shift register using a multiplexing technique.

### Required components

- UNO R3 Controller Board
- 4 Digit 7-Segment Display
- IR Receiver Module
- Remote Control
- Passive Buzzer
- 74HC595 IC
- 5 resistors 220 Ohm
- Breadboard
- Breadboard jumpers

### How to run

1. Install the **IRremote** library (version 2.7.0) in the Arduino IDE.
2. Open `Timer.ino`, select the "Arduino UNO" board and the correct serial port.
3. Wire the components according to the included circuit diagram (`Timer-Tinkercad.png` / `Timer.pdf`).
4. Upload the sketch to the board.

### Key features

- Digit-by-digit time setting via the IR remote, with position selection (up/down) and digit entry (0-9 number keys); the tens-of-seconds digit is capped at 5, consistent with the mm:ss format
- Start/pause of the countdown and overall power on/off, both via the remote
- Sound alarm (passive buzzer) when the timer reaches zero, with volume adjustable via the remote (it changes the frequency of the generated signal)
- Multiplexed 4-digit 7-segment display driven through a 74HC595 shift register, with the digit being edited blinking during time-setting mode
- Decoding of received IR codes via the IRremote library, with the remote's button mapping documented in `decodifica_tasti.txt`
- Diagnostic logging over the Serial Monitor (9600 baud) for debugging received IR codes and time changes

### Project structure

```
Timer-Arduino/
├── Timer.ino                 # Main sketch: IR handling, multiplexed display, buzzer, timer logic
├── decodifica_tasti.txt      # Mapping of the remote's IR codes to their respective buttons
├── Timer-Tinkercad.png       # Circuit diagram
└── Timer.pdf                 # Circuit diagram (Tinkercad export)
```

### Notes

Educational/personal project with an Arduino UNO R3; requires version 2.7.0 of the IRremote library (newer versions have a different API for signal decoding). The remote's codes are hardcoded in the source and are specific to the remote control model used (documented in `decodifica_tasti.txt`): they need to be redefined for a different remote.

### License

MIT
