const URL_SERVER = 'ws://localhost:3000'
const client = mqtt.connect(URL_SERVER)
let addValue = 0;

client.on('connect', (data) => {
  console.log(data)
  client.subscribe("/temperature")
})

client.on("message", (topic, payload) => {
  addValue = [payload].join("")
  console.log(addValue)
})

const ctx = document.getElementById("chart").getContext('2d')
const onRefresh = () => {
  config.data.datasets.forEach((dataset) => {
    dataset.data.push({
      x: moment(),
      y: addValue
    })
  })
}

const config = {
  type: 'line',
  data: {
    datasets: [{
      backgroundColor: '#FF1744',
      borderColor: '#FF1744',
      fill: false,
      label: "THD index (%)",
      cubicInterpolationMode: 'monotone',
      data: []
    }]
  },
  options: {
    responsive: true,
    title: {
      display: true,
      fontSize: 18,
      text: 'THD measurement with current meter circuit'
    },
    scales: {
      xAxes: [{
        type: 'realtime',
        display: true,
      }],
      yAxes: [{
        type: 'linear',
        display: true,
        scaleLabel: {
          display: true,
          labelString: 'THD (%)',
	  fontSize: 24
        }
      }]
    },
    tooltips: {
      intersect: false
    },
    hover: {
      mode: 'nearest',
      intersect: false
    },
    plugins: {
      streaming: {
        duration: 20000,
        delay: 2000,
        onRefresh,
      }
    }
  }
}
const chart = new Chart(ctx, config);
