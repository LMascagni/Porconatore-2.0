# Porconatore-2.0

## Descrizione del Progetto

Porconatore-2.0 è un'applicazione C che genera frasi umoristiche e offensive basate su santi e bestemmie. L'applicazione è progettata per funzionare su sistemi Windows e utilizza una GUI per interagire con l'utente. Gli utenti possono generare santi casuali, santi del giorno, e frasi offensive basate su santi specifici.

## Struttura del Progetto

Il progetto è organizzato come segue:

- `.vscode/`: Contiene i file di configurazione per Visual Studio Code.
  - `c_cpp_properties.json`: Configurazione del compilatore C/C++.
  - `launch.json`: Configurazione per il debug.
  - `settings.json`: Impostazioni specifiche per il progetto.
  - `tasks.json`: Configurazione dei task di build.

- `old/`: Contiene versioni precedenti e file di backup.
  - `a.exe`: Eseguibile di una versione precedente.
  - `grt/`: Contiene file di una versione precedente.
    - `a.exe`: Eseguibile di una versione precedente.
    - `main_grt.c`: Codice sorgente di una versione precedente.
  - `main.c`: Codice sorgente di una versione precedente.

- `src/`: Contiene il codice sorgente principale e le risorse.
  - `build.bat`: Script di build per compilare il progetto.
  - `data/`: Contiene file di dati utilizzati dall'applicazione.
    - `bestemms.txt`: Contiene termini offensivi.
    - `file_paths.txt`: Contiene i percorsi dei file di risorse.
    - `numeric_resources.txt`: Contiene risorse numeriche.
    - `santi.txt`: Contiene i nomi dei santi.
    - `string_resources.txt`: Contiene risorse di stringhe.
  - `img/`: Contiene le immagini utilizzate dall'applicazione.
  - `lib/`: Contiene i file di libreria.
    - `controls.c`: Implementazione dei controlli della finestra.
    - `controls.h`: Header per i controlli della finestra.
    - `file_manager.c`: Implementazione del gestore dei file.
    - `file_manager.h`: Header per il gestore dei file.
    - `resource_manager.c`: Implementazione del gestore delle risorse.
    - `resource_manager.h`: Header per il gestore delle risorse.
    - `stdbestemms.c`: Implementazione del generatore di bestemmie.
    - `stdbestemms.h`: Header per il generatore di bestemmie.
    - `stdsaint.c`: Implementazione del gestore dei santi.
    - `stdsaint.h`: Header per il gestore dei santi.
    - `error_messages.h`: Header per i messaggi di errore.
  - `main.c`: Codice sorgente principale dell'applicazione.
  - `Porconatore.exe`: Eseguibile dell'applicazione.

- `README.md`: Descrizione del progetto.

## Funzionalità Principali

- **Generazione di Santi Casuali**: L'utente può generare un santo casuale premendo un pulsante.
- **Santo del Giorno**: L'utente può ottenere il santo del giorno corrente.
- **Inserimento Data**: L'utente può inserire una data specifica per ottenere il santo corrispondente.
- **Generazione di Frasi Offensive**: L'applicazione può generare frasi offensive basate sui santi.

## Requisiti di Sistema

- Sistema Operativo: Windows
- Compilatore: GCC (MinGW)
- IDE: Visual Studio Code (opzionale)

## Istruzioni per la Build

1. Clonare il repository.
2. Aprire il progetto in Visual Studio Code.
3. Eseguire lo script di build `src/build.bat` per compilare l'applicazione.

## Utilizzo

1. Eseguire `Porconatore.exe` per avviare l'applicazione.
2. Utilizzare il menu e i pulsanti per generare santi e frasi offensive.

## Contributi

I contributi sono benvenuti! Sentitevi liberi di aprire issue e pull request per migliorare il progetto.

## Licenza

Questo progetto è distribuito sotto la licenza MIT. Vedere il file `LICENSE` per maggiori dettagli.