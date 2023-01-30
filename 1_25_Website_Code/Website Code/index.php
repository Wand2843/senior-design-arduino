<?php
$dsn = "mysql:host=senior-design.cwk33wgniqe5.us-east-1.rds.amazonaws.com;port=3306;user=admin;password=seniordesign7034#;charset=utf8mb4";
$pdo = new PDO($dsn);

$statement = $pdo->prepare('SELECT * FROM sys.test');
$statement->execute();
# $statement = $pdo->prepare('SELECT * FROM sensors;');
# $statement->execute();
# $sensors = $statement->fetchAll();
# foreach($sensors as $sensors) {
#	echo '<p>Sensor '. $sensor['name'] .': '. $sensor['value'] .'</p>'
# }


$test = $statement->fetch();

$sensorName = "Test Sensor";

# foreach($test as $it) {
#	echo $it;
#}

?>

<script src="https://cdn.tailwindcss.com"></script>

<div class="blob"></div>

<style>

.blob {
	background: rgba(0, 112, 0, 1);
	border-radius: 50%;
	margin: 10px;
	height: 20px;
	width: 20px;

	box-shadow: 0 0 0 0 rgba(0, 112, 0, 1);
	transform: scale(1);
	animation: pulse 2s infinite;
}

@keyframes pulse {
	0% {
		transform: scale(0.95);
		box-shadow: 0 0 0 0 rgba(0, 112, 0, 0.7);
	}

	70% {
		transform: scale(1);
		box-shadow: 0 0 0 10px rgba(0, 112, 0, 0);
	}

	100% {
		transform: scale(0.95);
		box-shadow: 0 0 0 0 rgba(0, 112, 0, 0);
	}
}


</style>

<div class="px-5">
  <img class="inline" src="https://i.ibb.co/2N34n9F/ece-Senior-Copy-2.png" alt="Girl in a jacket" width="200" height="300">
  <h3 class="text-lg font-medium leading-6 text-gray-900 inline">ECE 2023 Senior Design</h3>
  <dl class="mt-5 grid grid-cols-1 gap-5 sm:grid-cols-3">
  
    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Battery Temperature</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900">50</dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Current Reading</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900">50</dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Humidity</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900">50</dd>
    </div>
	
	    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Wind Speed</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900">50</dd>
    </div>
	
	    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Temperature</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900">50</dd>
    </div>
	
	    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Pressure</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900">50</dd>
    </div>
  </dl>
</div>