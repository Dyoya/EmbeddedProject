import express from "express"
import mysql from "mysql"

const app = express()
const port = 3020

const db = mysql.createConnection({
	host: 'svc.sel4.cloudtype.app',
    port: 30640,
	user: 'root',
	password: 'mysql1234',
	database: 'sensor',
})

db.connect()
app.use(express.urlencoded({extended: true}))
app.use(express.json())

app.get('/', (req, res) => {
    res.json({result: "스앱"})
})


app.get('/temperature', (req, res) => {
    const sql = 'select * from temperature'
    
    db.query(sql, (err, rows) => {
        if (err) {
            res.json({result: "error"})
            return console.log(err)
        }
        res.json(rows)
    })
})

app.post('/test',(req,res)=>{
    const sql = 'insert into temperature(value,time) values(?,?)'
    var params=[req.body.value,req.body.time]
    db.query(sql,params, (err, rows,fields) => {
        console.log(rows)
        if (err) {
            res.json({result: "error"})
            return console.log(err)
        }
        res.json(rows)
    })
    // console.log(req.body.value)
    // console.log(req.body.time)
    // res.send(req.body)
})

app.listen(port, () => {
    console.log(`서버 실행됨 (port ${port})`)
})