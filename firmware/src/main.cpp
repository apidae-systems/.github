#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

const char *ssid = "IT HURTS WHEN IP";
const char *password = "ithurtswhenip";

WebServer server(80);

#ifndef LED_PIN
#define LED_PIN 13
#endif

String formatUptime(unsigned long ms) {
  unsigned long s = ms / 1000UL;
  unsigned long d = s / 86400UL;
  s %= 86400UL;
  unsigned long h = s / 3600UL;
  s %= 3600UL;
  unsigned long m = s / 60UL;
  s %= 60UL;
  char buf[48];
  if (d)
    snprintf(buf, sizeof(buf), "%lud %luh %lum %lus", d, h, m, s);
  else if (h)
    snprintf(buf, sizeof(buf), "%luh %lum %lus", h, m, s);
  else if (m)
    snprintf(buf, sizeof(buf), "%lum %lus", m, s);
  else
    snprintf(buf, sizeof(buf), "%lus", s);
  return String(buf);
}

void handleRoot() {
  digitalWrite(LED_PIN, HIGH);

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Apidae Systems - ESP32</title>
<meta name="theme-color" content="#FFD400">
<style>
  :root{
    --bg:#0a0a0a;
    --card:#111213;
    --accent:#ffd400;
    --muted:#c8c8c8;
    --text:#f7f7f7;
    --ring:rgba(255,212,0,.35);
    --shadow: 0 20px 40px rgba(0,0,0,.6), 0 0 40px rgba(255,212,0,.08) inset;
  }
  *{box-sizing:border-box}
  html,body{height:100%;}
  body{
    margin:0;
    font-family: ui-sans-serif, system-ui, -apple-system, Segoe UI, Roboto, "Helvetica Neue", Arial, "Noto Sans", "Apple Color Emoji","Segoe UI Emoji";
    background: radial-gradient(1200px 600px at 80% -10%, rgba(255,212,0,.08), transparent 70%),
                radial-gradient(900px 500px at -10% 110%, rgba(255,212,0,.06), transparent 70%),
                var(--bg);
    color:var(--text);
    display:flex; align-items:center; justify-content:center;
    padding:24px;
  }
  .wrap{width:100%; max-width: 820px;}
  .badge{
    display:inline-flex; gap:8px; align-items:center;
    border:1px solid var(--ring); border-radius:9999px;
    padding:6px 12px; background:rgba(255,212,0,.06);
    box-shadow:0 0 0 3px rgba(255,212,0,.06) inset;
    font-weight:600; color:var(--accent); letter-spacing:.2px;
    text-transform:uppercase; font-size:12px;
  }
  .card{
    margin-top:18px;
    background: linear-gradient(180deg, rgba(255,212,0,.03), rgba(0,0,0,.0) 28%), var(--card);
    border:1px solid rgba(255,255,255,.07);
    border-radius:18px; padding:28px;
    box-shadow: var(--shadow);
    position:relative; overflow:hidden;
  }
  .glow{
    position:absolute; inset:-40%;
    background: radial-gradient(closest-side, rgba(255,212,0,.08), transparent);
    filter: blur(60px); pointer-events:none;
  }
  h1{margin:0 0 6px 0; font-size: clamp(28px, 4vw, 40px); letter-spacing:.3px;}
  .sub{color:var(--muted); margin:0 0 20px 0}
  .grid{
    display:grid; grid-template-columns: repeat(2,minmax(0,1fr)); gap:14px;
  }
  @media(max-width:560px){ .grid{grid-template-columns:1fr;} }
  .tile{
    background: rgba(255,212,0,.04);
    border:1px solid rgba(255,255,255,.08);
    border-radius:14px; padding:16px 16px 14px;
    backdrop-filter: blur(2px);
  }
  .k{font-size:12px; color:var(--muted); text-transform:uppercase; letter-spacing:.16em}
  .v{margin-top:6px; font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace;
     font-weight:700; font-size: clamp(16px, 2.8vw, 22px);
     text-shadow: 0 0 18px rgba(255,212,0,.18);
     color:var(--accent);
  }
  .footer{
    display:flex; gap:10px; align-items:center; justify-content:space-between; margin-top:18px; flex-wrap:wrap;
    color:var(--muted);
  }
  .btn{
    appearance:none; border:1px solid var(--accent); border-radius:12px;
    background:linear-gradient(180deg, rgba(255,212,0,.15), rgba(255,212,0,.06));
    color:#111; font-weight:800; letter-spacing:.3px;
    padding:10px 14px; cursor:pointer;
    box-shadow:0 8px 18px rgba(255,212,0,.18);
    transition:transform .08s ease, box-shadow .2s ease, filter .2s ease;
  }
  .btn:active{ transform: translateY(1px) scale(.99); }
  .dot{
    width:10px; height:10px; border-radius:9999px; background:var(--accent);
    box-shadow:0 0 0 4px rgba(255,212,0,.25), 0 0 18px rgba(255,212,0,.45);
  }
  .mono{font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace;}
  .hr{height:1px; background:linear-gradient(90deg, transparent, rgba(255,212,0,.25), transparent); margin:16px 0;}
</style>
</head>
<body>
  <div class="wrap">
    <span class="badge"><span class="dot"></span>ONLINE</span>
    <div class="card">
      <div class="glow"></div>
      <h1>By <span class="mono">Apidae Systems</span></h1>
      <p class="sub">Self-hosted stats, blazing minimal, Cloudflare-ready.</p>

      <div class="grid">
        <div class="tile"><div class="k">IP Address</div><div class="v)">%%IP%%</div></div>
        <div class="tile"><div class="k">Signal (RSSI)</div><div class="v">%%RSSI%% dBm</div></div>
        <div class="tile"><div class="k">Uptime</div><div class="v">%%UPTIME%%</div></div>
        <div class="tile"><div class="k">Free Heap</div><div class="v">%%HEAP%% B</div></div>
      </div>

      <div class="hr"></div>

      <div class="footer">
        <div>mDNS: <span class="mono">esp32.local</span></div>
        <button class="btn" onclick="location.reload()">Refresh</button>
      </div>
    </div>
  </div>
</body>
</html>
)rawliteral";

  html.replace("%%IP%%", WiFi.localIP().toString());
  html.replace("%%RSSI%%", String(WiFi.RSSI()));
  html.replace("%%UPTIME%%", formatUptime(millis()));
  html.replace("%%HEAP%%", String(ESP.getFreeHeap()));

  server.sendHeader("Cache-Control",
                    "no-store, no-cache, must-revalidate, max-age=0");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Content-Encoding", "identity");
  server.send(200, "text/html; charset=utf-8", html);

  digitalWrite(LED_PIN, LOW);
}

void handleNotFound() {
  digitalWrite(LED_PIN, HIGH);
  String message = "404 — Nothing here\n\nURI: " + server.uri();
  server.send(404, "text/plain; charset=utf-8", message);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  delay(150);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);

  Serial.print("\nConnecting");
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connect timeout. Check 2.4GHz/WPA2 and password.");
  }

  if (MDNS.begin("esp32")) {
    Serial.println("mDNS responder started (esp32.local)");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(2);
}
