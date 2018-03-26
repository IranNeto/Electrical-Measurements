const URL_SERVER = 'ws://localhost:3000'
const client = mqtt.connect(URL_SERVER)
let addValue = 0;
var ih;
var I;
var ifd;
var thd;
var freq;

client.on('connect', (data) => {
  client.subscribe("/temperature")
})

client.on("message",(topic, payload) => {
	const msg = JSON.parse(payload.toString());
	console.log(msg); 
    ih = msg.ih;
    I = msg.I;
    ifd = msg.ifd;
    thd = msg.thd;
    freq = msg.freq;
})

const ctx = document.getElementById("chart").getContext('2d')
const ctx2 = document.getElementById("chart2").getContext('2d')
/*
const onRefresh = () => {
  config.data.datasets.forEach((dataset) => {
    dataset.data.push({
      x: moment(),
      y: addValue
    })
  })
}
*/
var chartColors = {
			red: 'rgb(255, 99, 132)',
			orange: 'rgb(255, 159, 64)',
			yellow: 'rgb(255, 205, 86)',
			green: 'rgb(75, 192, 192)',
			blue: 'rgb(54, 162, 235)',
			purple: 'rgb(153, 102, 255)',
			grey: 'rgb(201, 203, 207)',
			black: 'rgb(0,0,0)'
		};

		function newDate(ms) {
			return moment().add(ms, 'ms');
		}

		function randomScalingFactor() {
			return (Math.random() > 0.5 ? 1.0 : -1.0) * Math.round(Math.random() * 100);
		}

		function onRefresh(){
			config.data.datasets[0].data.push({ x: moment(), y: ih});
			config.data.datasets[1].data.push({ x: moment(), y: ifd});
			config.data.datasets[2].data.push({ x: moment(), y: I});
		}

		var color = Chart.helpers.color;
		var config = {
			type: 'line',
			data: {
				datasets: [{
					label: 'Ih',
					backgroundColor: color(chartColors.red).alpha(0.5).rgbString(),
					borderColor: chartColors.red,
					fill: false,
					cubicInterpolationMode: 'monotone',
					data: []
				}, {
					label: 'Ifund',
					backgroundColor: color(chartColors.blue).alpha(0.5).rgbString(),
					borderColor: chartColors.blue,
					fill: false,
					cubicInterpolationMode: 'monotone',
					data: []
				},
				{
					label: 'I',
					backgroundColor: color(chartColors.gray).alpha(1).rgbString(),
					borderColor: chartColors.black,
					fill: false,
					cubicInterpolationMode: 'monotone',
					data: []
				}]
			},
			options: {
				responsive: true,
				title: {
					display: true,
					text: 'Line chart (hotizontal scroll) sample'
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
							labelString: 'value'
						}
					}]
				},
				tooltips: {
					mode: 'nearest',
					intersect: false
				},
				hover: {
					mode: 'nearest',
					intersect: false
				},
				plugins: {
					streaming: {
						duration: 20000,
						refresh: 1700,
						delay: 2500,
						onRefresh: onRefresh
					}
				}
			}
		};

const chart = new Chart(ctx, config);
//const chart2 = new Chart(ctx2, config)
