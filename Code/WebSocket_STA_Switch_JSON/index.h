const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>ESP32 Web Server</title>
    <style>
      .switch {
        position: relative;
        display: inline-block;
        width: 10rem;
        height: 5rem;
      }
      .switch input {
        display: none;
      }
      .slider {
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        border-radius: 3rem;
      }
      .slider:before {
        position: absolute;
        content: "";
        height: 4rem;
        width: 5rem;
        left: 0.5rem;
        bottom: 0.5rem;
        background-color: white;
        -webkit-transition: 0.4s;
        transition: 0.4s;
        border-radius: 3rem;
      }
      input:checked + .slider {
        background-color: #2196f3;
      }
      input:checked + .slider:before {
        -webkit-transform: translateX(4rem);
        -ms-transform: translateX(4rem);
        transform: translateX(4rem);
      }
      h1 {
        margin: 2rem;
        font-size: 3rem;
      }
      p {
        font-size: 2rem;
      }
    </style>
  </head>
  <body align="center">
    <h1>ESP32 Web Server</h1>
    <p id="LED">LED OFF</p>
    <label class="switch">
      <input onchange="Send_Data(this)" type="checkbox" id="output" />
      <span class="slider round"></span>
    </label>
    <script>
      var websocket = new WebSocket("ws://" + location.hostname + ":81/");
      websocket.onopen = function (event) {
        onOpen(event);
      };
      websocket.onclose = function (event) {
        onClose(event);
      };
      websocket.onmessage = function (event) {
        onMessage(event);
      };
      websocket.onerror = function (event) {
        onError(event);
      };
      function Send_Data(element) {
        var command;
        if (element.checked) command = '{"LED":"ON"}';
        else command = '{"LED":"OFF"}';
        websocket.send(command);
      }
      function onOpen(event) {
        console.log("Server Connected!");
        alert("Server Connected!");
      }
      function onClose(event) {
        console.log("Server Disconnected!");
        alert("Server Disconnected!");
      }
      function onMessage(event) {
        var message;
        var Switch_Status;
        var JSONContent = JSON.parse(event.data);
        if (JSONContent.LED == "ON") {
          message = "LED ON";
          Switch_Status = true;
        } else {
          message = "LED OFF";
          Switch_Status = false;
        }
        console.log(message);
        document.getElementById("LED").innerHTML = message;
        document.getElementById("output").checked = Switch_Status;
      }
      function onError(event) {
        console.log("Error:" + event.data);
        alert("Error Occured!");
      }
    </script>
  </body>
</html>
)rawliteral";
