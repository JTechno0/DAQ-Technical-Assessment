import net from "net";
import { WebSocket, WebSocketServer } from "ws";

interface VehicleData {
  battery_temperature: number;
  timestamp: number;
}

const TCP_PORT = 12000;
const WS_PORT = 8080;
const tcpServer = net.createServer();
const websocketServer = new WebSocketServer({ port: WS_PORT });
let parsedMsg;
var TempTemp = new Array();
var TempTime = new Array();
tcpServer.on("connection", (socket) => {
  console.log("TCP client connected");

  socket.on("data", (msg) => {
    console.log(`Received: ${msg.toString()}`);
    parsedMsg = msg.toString();
    let noB = parsedMsg;
    let end = false;
    //loop to run until all {} are removed
    while(!end)
    {
      if(noB.indexOf("{") != -1 && noB.lastIndexOf("}") != -1)
      {
        //removes brackets in pairs
        noB = noB.slice(noB.indexOf("{")+1,noB.lastIndexOf("}"));
      } else {
        if(noB.indexOf("{") == -1 && noB.lastIndexOf("}") == -1)
        {
          //add brackets back to make it a json string
          noB = "{" + noB + "}";
          end = true;
        } else if(noB.indexOf("{") != -1)
        {
          //removes brackets in singles
          noB = noB.replace("{","");
        } else if(noB.lastIndexOf("}") != -1)
        {
          //removes brackets in singles
          noB = noB.replace("}","");
        }
      }
    }
    var jsonDat = JSON.parse(noB);
    
    const jsonData: VehicleData = JSON.parse(noB);
    //checks if data is out of range
    if(jsonData.battery_temperature>80 || jsonData.battery_temperature<20)
    {
      //checks if it can compare to 3 values
      if(TempTemp.length == 3)
      {
      TempTemp.shift();
      TempTemp.push(jsonData.battery_temperature);
      TempTime.shift();
      TempTime.push(jsonData.timestamp);
      //checks how long the first value and second value are apart
      if((TempTime[2]-TempTime[0])/1000<=5)
      {
        console.log(TempTime);
        console.log(TempTemp);
        console.log("Time: " + Date.now() + ": Operating Temperature Issue");
        TempTime = [];
        TempTemp = [];
      }
      //adds value to array till we get to three values
      } else {
        TempTemp.push(jsonData.battery_temperature);
        TempTime.push(jsonData.timestamp);
        if(TempTemp.length == 3) {
          //checks how long the first value and second value are apart
          if((TempTime[2]-TempTime[0])/1000<=5)
          {
            console.log(TempTime);
            console.log(TempTemp);
            console.log(Date.now() + ": Temperature Error");
            TempTime = [];
            TempTemp = [];
          }
        }
      }
    }
    // Send JSON over WS to frontend clients
    websocketServer.clients.forEach(function each(client) {
      if (client.readyState === WebSocket.OPEN) {
        client.send(noB);
      }
    });
  });

  socket.on("end", () => {
    console.log("Closing connection with the TCP client");
  });

  socket.on("error", (err) => {
    console.log("TCP client error: ", err);
  });
});

websocketServer.on("listening", () =>
  console.log(`Websocket server started on port ${WS_PORT}`)
);

websocketServer.on("connection", async (ws: WebSocket) => {
  console.log("Frontend websocket client connected");
  ws.on("error", console.error);
});

tcpServer.listen(TCP_PORT, () => {
  console.log(`TCP server listening on port ${TCP_PORT}`);
});
