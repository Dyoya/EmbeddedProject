import express from "express";
import mysql from "mysql";

const app = express();
const port = 3020;

const db = mysql.createConnection({
  host: "svc.sel4.cloudtype.app",
  port: 30640,
  user: "root",
  password: "mysql1234",
  database: "sensor",
});

db.connect();
app.use(express.urlencoded({ extended: true }));
app.use(express.json());

app.get("/", (req, res) => {
  res.json({ result: "스앱" });
});

var result = false;

app.get("/warning", (req, res) => {
  if (result == false) {
    result = true;
    res.json({ warning: "Get Ok" });
    console.log(result);
  }
});

app.get("/buzzer", (req, res) => {
  if (result) {
    res.json({ warning: "noise" });
    result = false;
  } else {
    res.json({ warning: "quiet" });
  }
  console.log(result);
});

app.get("/sensor", (req, res) => {
  const sql = "select * from sensor";
  db.query(sql, (err, rows) => {
    if (err) {
      res.json({ result: "error" });
      return console.log(err);
    }
    res.json(rows);
  });
});

app.post("/sensor", (req, res) => {
  const sql = "update sensor set temperature=?, water=?, gas=?, nfc=?";
  var params = [
    req.body.temperature,
    req.body.water,
    req.body.gas,
    req.body.nfc,
  ];
  console.log(params);
  db.query(sql, params, (err, rows, fields) => {
    console.log(rows);
    if (err) {
      res.json({ result: "error" });
      return console.log(err);
    }
    res.json(rows);
  });
});

app.listen(port, () => {
  console.log(`서버 실행됨 (port ${port})`);
});
