const URL_SERVER = 'ws://localhost:3000'
const client = mqtt.connect(URL_SERVER)
var ih;
var I;
var ifd;
var thd;
var freq;

client.on('connect', (data) => {
	client.subscribe("/temperature")
})

client.on("message",(topic, payload) => {
	//O ParseErros acontece quando o ESP manda algum valor inválido: NaN, inf.
	console.log(payload.toString());
	const msg = JSON.parse(payload.toString());
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

function onRefresh2(){
	config2.data.datasets[0].data.push({ x: moment(), y: thd});
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
				//duration: 20000,
				refresh: 1000,
				delay: 900,
				onRefresh: onRefresh
			}
		}
	}
};

var config2 = {
	type: 'line',
	data: {
		datasets: [{
			label: 'Ih',
			backgroundColor: color(chartColors.green).alpha(0.2).rgbString(),
			borderColor: chartColors.green,
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
				//duration: 20000,
				refresh: 1200,
				delay: 900,
				onRefresh: onRefresh2
			}
		}
	}
};
//Olhar depois
//https://www.highcharts.com/demo/gauge-solid

Highcharts.chart('container', {

	chart: {
			type: 'gauge',
			plotBackgroundColor: null,
			plotBackgroundImage: null,
			plotBorderWidth: 0,
			plotShadow: false
	},

	title: {
			text: 'Signal\'s frequency'
	},

	pane: {
			startAngle: -90,
			endAngle: 90,
			background: [{
					backgroundColor: {
							linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
							stops: [
									[0, '#FFF'],
									[1, '#333']
							]
					},
					borderWidth: 0,
					outerRadius: '119%'
			}, {
					backgroundColor: {
							linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
							stops: [
									[0, '#333'],
									[1, '#FFF']
							]
					},
					borderWidth: 1,
					outerRadius: '107%'
			}, {
					// default background
			}, {
					backgroundColor: '#DDD',
					borderWidth: 0,
					outerRadius: '105%',
					innerRadius: '103%'
			}]
	},

	// the value axis
	yAxis: {
			min: 0,
			max: 180,

			minorTickInterval: 'auto',
			minorTickWidth: 1,
			minorTickLength: 10,
			minorTickPosition: 'inside',
			minorTickColor: '#666',

			tickPixelInterval: 30,
			tickWidth: 2,
			tickPosition: '',
			tickLength: 10,
			tickColor: '#666',
			labels: {
					step: 2,
					rotation: 'auto'
			},
			title: {
					text: 'Hz'
			},
			plotBands: [{
					from: 0,
					to: 60,
					color: '#55BF3B' // green
			}, {
					from: 60,
					to: 120,
					color: '#DDDF0D' // yellow
			}, {
					from: 120,
					to: 200,
					color: '#DF5353' // red
			}]
	},

	series: [{
			name: 'Frequency',
			data: [0],
			tooltip: {
					valueSuffix: 'Hz'
			}
	}]

},
// Add some life
function (chart) {
	if (!chart.renderer.forExport) {
			setInterval(function () {
					var point = chart.series[0].points[0],
							newVal,
							inc = Math.round((Math.random() - 0.5) * 20);

					newVal = point.y + inc;
					if (newVal < 0 || newVal > 200) {
							newVal = point.y - inc;
					}

					point.update(freq);

			}, 1000);
	}
});

const chart = new Chart(ctx, config);
const chart2 = new Chart(ctx2, config2);