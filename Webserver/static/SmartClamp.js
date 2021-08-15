
var endTime=0;
var startTime=0;
var clicks1=0;
var clicks2=0;
var clicks3=0;
var clicks4=0;
var clicks5=0;
var clicks6=0;
var clicks7=0;
var clicks8=0;
var TimeDiff=0;
var data_final=[];
var time_points=[];
let IntervalFunction1;
let IntervalFunction2;
let IntervalFunction3;
let IntervalFunction4;
let IntervalFunction5;
let IntervalFunction6;
let IntervalFunction7;
let IntervalFunction8;

//Function to calculate time elapsed, create live plot

//$('lineChart1').hide(); //Do not show plot until Start/Stop is pressed
$('buttonone').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks1++;

	var dropdownvalue1="";
  var e = document.getElementById("SelectedPlotFlask1");
	dropdownvalue1 = e.options[e.selectedIndex].text; //read the drop down menu value
//	console.log(dropdownvalue1);
  //alert(clicks);
    if (clicks1 == 1)
  {

    var Email1 = document.getElementById('Email1').value
    if (Email1 == "")
    {
      alert("Please enter your email address!")
    }

    startTime = new Date();
    //$('lineChart1').show();

var canvas = document.getElementById('lineChart1');
var data = {
  labels: [0, 1, 2, 3, 4, 5],
  datasets: [
    {
      label: dropdownvalue1,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40],
    }
  ]
};

var data_points1 = 6;

IntervalFunction1 = setInterval(function(){
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points1);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data;
  //console.log(data_final);
	time_points = myLineChart.data.labels;
 // console.log(time_points);
  myLineChart.update();
  data_points1++;
},1000);

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks1 == 2)
  {
  	clearInterval(IntervalFunction1);
    
  	endTime = new Date();
    TimeDiff = endTime - startTime;
    TimeDiff /= 1000;

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

//		console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart1');
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue1,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

//Function to calculate time elapsed, create live plot
//$('lineChart2').hide(); //Do not show plot until Start/Stop is pressed
$('buttontwo').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks2++;

	var dropdownvalue2="";
	var e = document.getElementById("SelectedPlotFlask2");
	dropdownvalue2 = e.options[e.selectedIndex].text; //read the drop down menu value
	//console.log(dropdownvalue2);
  //alert(clicks);
    if (clicks2 == 1) //check if the user wishes to start the experiment (number of clicks  = 1)
  {
    var Email2 = document.getElementById('Email1').value
    if (Email2 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
    //$('lineChart2').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart2');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue2,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points2 = 6;

IntervalFunction2 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points2);
//  myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points2++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks2 == 2) //check to see whether user wishes to Stop the experiemnt (clicks  = 2)
  {
  	clearInterval(IntervalFunction2); //pause the live plot — stop adding data + changing the x-axis after every 1 second

		endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
    // alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart2'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue2,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

//Function to calculate time elapsed, create live plot
//$('lineChart3').hide(); //Do not show plot until Start/Stop is pressed
$('buttonthree').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks3++;
	var dropdownvalue3="";
	var e = document.getElementById("SelectedPlotFlask3");
	dropdownvalue3 = e.options[e.selectedIndex].text; //read the drop down menu value
	//console.log(dropdownvalue3);
  //alert(clicks);
    if (clicks3 == 1) //check if the user wishes to start the experiment (number of clicks  = 1)
  {
    var Email3 = document.getElementById('Email1').value
    if (Email3 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
  //  $('lineChart3').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart3');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue3,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points3 = 6;

IntervalFunction3 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points3);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points3++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks3 == 2) //check to see whether user wishes to Stop the experiemnt (clicks  = 2)
  {
  	clearInterval(IntervalFunction3); //pause the live plot — stop adding data + changing the x-axis after every 1 second
  	endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart3'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue3,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

//Function to calculate time elapsed, create live plot
//$('lineChart4').hide(); //Do not show plot until Start/Stop is pressed
$('buttonfour').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks4++;
	var dropdownvalue4="";
	var e = document.getElementById("SelectedPlotFlask4");
	dropdownvalue4 = e.options[e.selectedIndex].text; //read the drop down menu value
	//console.log(dropdownvalue4);
  //alert(clicks);
    if (clicks4 == 1) //check if the user wishes to start the experiment (number of clicks = 1)
  {
    var Email4 = document.getElementById('Email1').value
    if (Email4 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
  //  $('lineChart4').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart4');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue4,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points4 = 6;

IntervalFunction4 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points4);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points4++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks4 == 2) //check to see whether user wishes to Stop the experiemnt (clicks  = 2)
  {
  	clearInterval(IntervalFunction4); //pause the live plot — stop adding data + changing the x-axis after every 1 second
  	endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart4'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue4,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

//Function to calculate time elapsed, create live plot
//$('lineChart5').hide(); //Do not show plot until Start/Stop is pressed
$('buttonfive').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks5++;
	var dropdownvalue5="";
	var e = document.getElementById("SelectedPlotFlask5");
	dropdownvalue5 = e.options[e.selectedIndex].text; //read the drop down menu value
//	console.log(dropdownvalue5);
  //alert(clicks);
    if (clicks5 == 1) //check if the user wishes to start the experiment (number of clicks  = 1)
  {
    var Email5 = document.getElementById('Email1').value
    if (Email5 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
//    $('lineChart5').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart5');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue5,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points5 = 6;

IntervalFunction5 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points5);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points5++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks5 == 2) //check to see whether user wishes to Stop the experiemnt (clicks  = 2)
  {
  	clearInterval(IntervalFunction5); //pause the live plot — stop adding data + changing the x-axis after every 1 second
  	endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart5'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue5,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

//Function to calculate time elapsed, create live plot
//$('lineChart6').hide(); //Do not show plot until Start/Stop is pressed
$('buttonsix').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks6++;
	var dropdownvalue6="";
	var e = document.getElementById("SelectedPlotFlask6");
	dropdownvalue6 = e.options[e.selectedIndex].text; //read the drop down menu value
	//console.log(dropdownvalue6);
  //alert(clicks);
    if (clicks6 == 1) //check if the user wishes to start the experiment (number of clicks  = 1)
  {
    var Email6 = document.getElementById('Email1').value
    if (Email6 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
  //  $('lineChart6').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart6');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue6,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points6 = 6;

IntervalFunction6 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points6);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points6++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks6 == 2) //check to see whether user wishes to Stop the experiemnt ( = 2)
  {
  	clearInterval(IntervalFunction6); //pause the live plot — stop adding data + changing the x-axis after every 1 second
  	endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart6'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue6,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});


//Function to calculate time elapsed, create live plot
//$('lineChart7').hide(); //Do not show plot until Start/Stop is pressed
$('buttonseven').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks7++;
	var dropdownvalue7="";
	var e = document.getElementById("SelectedPlotFlask7");
	dropdownvalue7 = e.options[e.selectedIndex].text; //read the drop down menu value
	//console.log(dropdownvalue7);
  //alert(clicks);
    if (clicks7 == 1) //check if the user wishes to start the experiment (number of clicks  = 1)
  {
    var Email7 = document.getElementById('Email1').value
    if (Email7 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
    //$('lineChart7').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart7');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue7,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points7 = 6;

IntervalFunction7 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points7);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points7++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks7 == 2) //check to see whether user wishes to Stop the experiemnt (clicks = 2)
  {
  	clearInterval(IntervalFunction7); //pause the live plot — stop adding data + changing the x-axis after every 1 second
  	endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart7'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue7,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

//Function to calculate time elapsed, create live plot
//$('lineChart8').hide(); //Do not show plot until Start/Stop is pressed
$('buttoneight').observe('click', function(event) { //check whether Start/Stop of Flask 1 is pressed
	clicks8++;
	var dropdownvalue8="";
	var e = document.getElementById("SelectedPlotFlask8");
	dropdownvalue8 = e.options[e.selectedIndex].text; //read the drop down menu value
//	console.log(dropdownvalue8);
  //alert(clicks);
    if (clicks8 == 1) //check if the user wishes to start the experiment (number of clicks = 1)
  {
    var Email8 = document.getElementById('Email1').value
    if (Email8 == "")
    {
      alert("Please enter your email address!")
    }
    startTime = new Date();
  //  $('lineChart8').show(); //display plot when Start is pressed

var canvas = document.getElementById('lineChart8');
var data = {
  labels: [0, 1, 2, 3, 4, 5], //x-axis values: time in seconds
  datasets: [
    {
      label: dropdownvalue8,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: [65, 59, 80, 0, 56, 55, 40], //preliminary data
    }
  ]
};

var data_points8 = 6;

IntervalFunction8 = setInterval(function(){ //add data and increase time in axis every second
 // adddata();
 var value = Math.floor((Math.random() * 100) + 1);
  myLineChart.data.labels.push(data_points8);
  //myLineChart.data.labels.splice(0, 1);
  //myLineChart.data.datasets[0].data.splice(0, 1);
  //console.log(myLineChart.data.datasets[0].data);
  myLineChart.data.datasets[0].data.push(value);
	//console.log(myLineChart.data.labels);
  data_final = myLineChart.data.datasets[0].data; //store the last data points
  //console.log(data_final);
	time_points = myLineChart.data.labels; //store the last values of the x axis
 // console.log(time_points);
  myLineChart.update();
  data_points8++;

},1000); //1000ms = 1s

var option = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart = Chart.Line(canvas,{
  data:data,
  options:option
});

  }
  else if (clicks8 == 2) //check to see whether user wishes to Stop the experiemnt (clicks = 2)
  {
  	clearInterval(IntervalFunction8); //pause the live plot — stop adding data + changing the x-axis after every 1 second
  	endTime = new Date();
    TimeDiff = endTime - startTime; //calculate time differnece
    TimeDiff /= 1000; //convert time from milliseconds to seconds

      // get seconds
     var seconds = Math.round(TimeDiff);
  //   alert('The time elapsed is ' + seconds + 's');

	//	console.log(data_final);
  //  console.log(time_points);

var canvas_new = document.getElementById('lineChart8'); //with the stored values, create static line chart
var data_new = {
  labels: time_points,
  datasets: [
    {
      label: dropdownvalue,
      fill: false,
      lineTension: 0.0,
      backgroundColor: "rgba(75,192,192,0.4)",
      borderColor: "rgba(75,192,192,1)",
      borderCapStyle: 'butt',
      borderDash: [],
      borderDashOffset: 0.0,
      borderJoinStyle: 'miter',
      pointBorderColor: "rgba(75,192,192,1)",
      pointBackgroundColor: "#fff",
      pointBorderWidth: 1,
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(75,192,192,1)",
      pointHoverBorderColor: "rgba(220,220,220,1)",
      pointHoverBorderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 10,
      data: data_final,
    }
  ]
};
var option_new = {
  showLines: true,
  scales: {
    yAxes: [{
      display: true,
      ticks: {
        beginAtZero:true,
        min: 0,
        max: 100
      }
    }],
		xAxes: [{
				 scaleLabel: {
					 display: true,
					 labelString: 'Time (s)'
				 }
			 }],
  }
};
var myLineChart_new = Chart.Line(canvas_new,{
  data:data_new,
  options:option_new
});

  }

});

var Battery_Level=0;

$('battery1_low').hide();
$('battery1_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery1_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery1_high').show();
		}

});

$('battery2_low').hide();
$('battery2_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery2_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery2_high').show();
		}

});

$('battery3_low').hide();
$('battery3_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery3_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery3_high').show();
		}

});

$('battery4_low').hide();
$('battery4_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery4_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery4_high').show();
		}

});

$('battery5_low').hide();
$('battery5_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery5_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery5_high').show();
		}

});

$('battery6_low').hide();
$('battery6_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery6_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery6_high').show();
		}

});

$('battery7_low').hide();
$('battery7_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery7_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery7_high').show();
		}

});

$('battery8_low').hide();
$('battery8_high').hide();

navigator.getBattery()
  .then(function(battery) {
  	Battery_Level = battery.level;

		if (Battery_Level < 0.5)
		{
			$('battery8_low').show();
		}
		else if (Battery_Level >= 0.5) {
			$('battery8_high').show();
		}

});

// to display slider value:
var slider1 = document.getElementById("Range1");
var output1 = document.getElementById("demo1");
output1.innerHTML = ((slider1.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
  slider1.oninput = function() {
  output1.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider2 = document.getElementById("Range2");
var output2 = document.getElementById("demo2");
output2.innerHTML = ((slider2.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider2.oninput = function() {
  output2.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider3 = document.getElementById("Range3");
var output3 = document.getElementById("demo3");
output3.innerHTML = ((slider3.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider3.oninput = function() {
  output3.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider4 = document.getElementById("Range4");
var output4 = document.getElementById("demo4");
output4.innerHTML = ((slider4.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider4.oninput = function() {
  output4.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider5 = document.getElementById("Range5");
var output5 = document.getElementById("demo5");
output5.innerHTML = ((slider5.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider5.oninput = function() {
  output5.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider6 = document.getElementById("Range6");
var output6 = document.getElementById("demo6");
output6.innerHTML = ((slider6.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider6.oninput = function() {
  output6.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider7 = document.getElementById("Range7");
var output7 = document.getElementById("demo7");
output7.innerHTML = ((slider7.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider7.oninput = function() {
  output7.innerHTML = ((this.value)/100).toFixed(2);
}

// to display slider value:
var slider8 = document.getElementById("Range8");
var output8 = document.getElementById("demo8");
output8.innerHTML = ((slider8.value)/100).toFixed(2); // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider8.oninput = function() {
  output8.innerHTML = ((this.value)/100).toFixed(2);
}
