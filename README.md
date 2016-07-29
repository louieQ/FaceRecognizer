# FaceRecognizer

FaceRecognizer è un sistema per il riconoscimento di parametri biometrici (genere, età stimata, gruppo etnico) basato su OpenCV 3.

## Modalità d'uso

Una volta compilato e dotato dei permessi di esecuzione, il programma può essere avviato da terminale con il seguente comando.

N.B.: si suppone che la cartella corrente di lavoro del terminale sia quella in cui è presente l'eseguibile.

```
./FaceRecognizer <input> <source> <detectionsRequired>
```

Dove:
- `<input>` ::= -vid | -img | -cam
- `<source>` ::=
  - il percorso del video o dell'immagine nel file system, se il primo argomento specificato è `-vid` o `-img`
  - l'id del dispositivo di acquisizione video (della webcam), se il primo argomento specificato è `-cam`.
- `<detectionsRequired>` := -g | -r | -a | -gh | -ah
  - `-g` richiede di identificare il *genere*
  - `-r` richiede di identificare l'*etnia*
  - `-a` richiede di identificare l'*età*
  - `-gh` richiede di identificare il *genere con un algoritmo di deep learning* (richiede una macchina ad elevate prestazioni)
  - `-ah` richiede di identificare l'*età con un algoritmo di deep learning* (richiede una macchina ad elevate prestazioni)
  
## Compilazione e dipendenze

Per poter compilare correttamente FaceRecognizer bisogna aver compilato e installato:
- OpenCV 3.1.0
- OpenCV extra modules

Il progetto, previa eliminazione del file .user, può essere facilmente importato nella IDE QTCreator in cui è stato sviluppato.

## Note

Alcuni modelli non sono stati inclusi in questa repository a causa della loro grande dimensione. Per questo motivo le funzioni che da essi dipendono non saranno momentaneamente disponibili. In particolare sono assenti i modelli che fanno uso del deep learning.
