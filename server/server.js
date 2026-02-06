const path = require('path');
const express = require('express');
const { execFile } = require('child_process');
const cors = require('cors');

const app = express();
const port = 5000;

app.use(cors());
app.use(express.json());

const calcPath = path.join(__dirname, '../calc');

app.post('/calculate', (req, res) => {
    const { args } = req.body;

    execFile(calcPath, args, (err, stdout, stderr) => {
        if (err) return res.status(500).send(stderr);
        const lines = stdout.trim().split('\n');
        const finalResult = lines[lines.length - 1];
        res.json({ result: finalResult })
    })
})

app.listen(5000, () => console.log('Sever running on port 5000'));