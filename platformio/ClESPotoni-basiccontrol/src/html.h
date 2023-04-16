#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ClESPotoni</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 2rem;
  }
  div {
    text-align: center;
  }
  button {
    height: 100px;
    width: 100px;
  }
  .wide {
    width: 200px;
  }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>ClESPotoni</h1>
  </div>
  <div class="content">
    <div>
      <p class="state">state: <span id="status">OFF</span></p>
    </div>
    <div>
      <p><button id="forw" class="wide" onmousedown="buttonOn('forw')" onmouseup="buttonOff('forw')" ontouchstart="buttonOn('forw')" ontouchend="buttonOff('forw')">&uarr;</button></p>
    </div>
    <div>
      <p>
        <button id="forw" onmousedown="buttonOn('left')" onmouseup="buttonOff('left')" ontouchstart="buttonOn('left')" ontouchend="buttonOff('left')">&#8634;</button>
        <button id="forw" onmousedown="buttonOn('righ')" onmouseup="buttonOff('righ')" ontouchstart="buttonOn('righ')" ontouchend="buttonOff('righ')">&#8635;</button>
      </p>
    </div>
    <div>
      <p><button id="forw" class="wide" onmousedown="buttonOn('back')" onmouseup="buttonOff('back')" ontouchstart="buttonOn('back')" ontouchend="buttonOff('back')">&darr;</button></p>
    </div>
    <div>
      <p><button id="forw" class="wide" onmousedown="buttonOn('beep')" onmouseup="buttonOff('beep')" ontouchstart="buttonOn('beep')" ontouchend="buttonOff('beep')">Beep</button></p>
    </div>
    </div>
  </div>
  <script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    window.addEventListener('load', onLoad);
    function initWebSocket() {
      console.log('Trying to open a WebSocket connection...');
      websocket = new WebSocket(gateway);
      websocket.onopen    = onOpen;
      websocket.onclose   = onClose;
      websocket.onmessage = onMessage;
    }
    function onOpen(event) {
      console.log('Connection opened');
    }
    function onClose(event) {
      console.log('Connection closed');
      setTimeout(initWebSocket, 2000);
    }
    function onMessage(event) {
      console.log(event.data);
      const data = JSON.parse(event.data);
      const element = document.getElementById(data.id);
      if (element) {
          element.innerHTML = data.value;
      }
    }
    function onLoad(event) {
      initWebSocket();
    }
    function buttonOn(direction){
      websocket.send(direction+'_on');
    }
    function buttonOff(direction){
      websocket.send(direction+'_off');
    }
  </script>
</body>
</html>
)rawliteral";