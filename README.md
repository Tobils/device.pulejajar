# ESP32

## Config

```bash
# build and upload clean, etc
python -m platformio run --target upload
# serial monitor
python -m platformio device monitor --baud 115200
```

## Contoh data yang dikirim

```json
// post data body
{
  "meta": {
    "token": "token"
  },
  "payload": {
    "data_sensor": {
      "suhu": 25,
      "tds": 150,
      "ph": 6,
      "debit": 300
    },
    "date": "2020:12:12",
    "time": "23:10:44"
  }
}
```

## Referensi

- [http client esp32](https://randomnerdtutorials.com/esp32-http-get-post-arduino/#http-post)
