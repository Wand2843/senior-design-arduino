<?php
$servername = "sql9.freemysqlhosting.net";
$username = "sql9596235";
$password = "7LCYhZVIwK";
$dbname = "sql9596235";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT temperature, pressure, altitude, humidity, updated_at FROM tracked_sensors";
$sql1 = "SELECT battery_soc, battery_voltage, battery_charging_amps, battery_temperature, controller_temperature, load_voltage, load_amps, load_watts, solar_panel_voltage, solar_panel_amps, solar_panel_watts, updated_at FROM battery_sensors";  
$result = $conn->query($sql);
$result1 = $conn->query($sql1);
$row = $result->fetch_assoc();
$row1 = $result1->fetch_assoc();
?>

<?php
    
    // Sample date string
    $dateString = $row["updated_at"];

    // Split the date and time
    list($arduino_date, $arduino_time) = explode(' ', $dateString);

    // Extract the year, month, and day from the date
    list($arduino_year, $arduino_month, $arduoino_day) = explode('-', $arduino_date);

    // Extract the hour, minute, and second from the time
    list($arduino_hour, $arduino_minute, $arduino_second) = explode(':', $arduino_time);

    date_default_timezone_set("America/New_York");
    $current_time = date("H:i:sa");

    list($current_hour, $current_minute, $current_second) = explode(':', $current_time);

    $arduino_hour = (int)$arduino_hour + 1;
    $arduino_minute = (int)$arduino_minute;
    $arduino_second = (int)$arduino_second;

    $current_hour = (int)$current_hour;
    $current_minute = (int)$current_minute;
    $current_second = (int)$current_second;

    // Calculate the Unix timestamps for the two times
    $timestamp1 = mktime($arduino_hour, $arduino_minute, $arduino_second);
    $timestamp2 = mktime($current_hour, $current_minute, $current_second);

    // Calculate the difference in seconds
    $diffSeconds = $timestamp2 - $timestamp1;

    // Output the result
    
?>


<!-- component -->
<script src="https://cdn.tailwindcss.com"></script>
<link rel="stylesheet" type="text/css" href="styles.css">

<aside class="ml-[-100%] fixed z-10 top-0 pb-3 px-6 w-full flex flex-col justify-between h-screen border-r bg-white transition duration-300 md:w-4/12 lg:ml-0 lg:w-[25%] xl:w-[20%] 2xl:w-[15%]">
    <div>
        <div class="-mx-6 px-6 py-1">
            <a href="#" title="home">
            </a>
        </div>

        <div class="text-center">
		    <img src="logo.png" alt="Italian Trulli">
			<div class="flex items-center justify-center">
            <?php

            if(($diffSeconds + 63 < 60) && ($diffSeconds + 63 > 0)) {
			    echo "<div class='blob'></div>";
			    echo "<span class='pulsate-text'>System Online</span>";
            }
            else if ($diffSeconds + 63 >= 60) {
                echo "<div class='blob-red'></div>";
                echo "<span class='pulsate-text-red'>System Offline</span>";    
            }
            else if ($diffSeconds < 0) {
                echo "<div class='blob-red'></div>";
                echo "<span class='pulsate-text-red'>System Offline</span>";    
            }

            ?>
			</div>

            <h5 class="hidden mt-4 text-xl font-semibold text-gray-600 lg:block">William Anderson</h5>
            <span class="hidden text-gray-400 lg:block">Admin</span>
        
        </div>

        <ul class="space-y-2 tracking-wide mt-8">
            <li>
                <a href="#" aria-label="dashboard" class="relative px-4 py-3 flex items-center space-x-4 rounded-xl text-white bg-gradient-to-r from-sky-600 to-cyan-400">
                    <svg class="-ml-1 h-6 w-6" viewBox="0 0 24 24" fill="none">
                        <path d="M6 8a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2H8a2 2 0 0 1-2-2V8ZM6 15a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2H8a2 2 0 0 1-2-2v-1Z" class="fill-current text-cyan-400 dark:fill-slate-600"></path>
                        <path d="M13 8a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2h-1a2 2 0 0 1-2-2V8Z" class="fill-current text-cyan-200 group-hover:text-cyan-300"></path>
                        <path d="M13 15a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2h-1a2 2 0 0 1-2-2v-1Z" class="fill-current group-hover:text-sky-300"></path>
                    </svg>
                    <span class="-mr-1 font-medium">Dashboard</span>
                </a>
            </li>
            <li>
                <a href="/profile.php" class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
                    <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor">
                        <path class="fill-current text-gray-300 group-hover:text-cyan-300" fill-rule="evenodd" d="M2 6a2 2 0 012-2h4l2 2h4a2 2 0 012 2v1H8a3 3 0 00-3 3v1.5a1.5 1.5 0 01-3 0V6z" clip-rule="evenodd" />
                        <path class="fill-current text-gray-600 group-hover:text-cyan-600" d="M6 12a2 2 0 012-2h8a2 2 0 012 2v2a2 2 0 01-2 2H2h2a2 2 0 002-2v-2z" />
                    </svg>
                    <span class="group-hover:text-gray-700">Profile</span>
                </a>
            </li>
            <li>
                <a href="/control.php" class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
                    <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor">
                        <path class="fill-current text-gray-600 group-hover:text-cyan-600" fill-rule="evenodd" d="M2 5a2 2 0 012-2h8a2 2 0 012 2v10a2 2 0 002 2H4a2 2 0 01-2-2V5zm3 1h6v4H5V6zm6 6H5v2h6v-2z" clip-rule="evenodd" />
                        <path class="fill-current text-gray-300 group-hover:text-cyan-300" d="M15 7h1a2 2 0 012 2v5.5a1.5 1.5 0 01-3 0V7z" />
                    </svg>
                    <span class="group-hover:text-gray-700">Control Center</span>
                </a>
            </li>
        </ul>
    </div>

    <div class="px-6 -mx-6 pt-4 flex justify-between items-center border-t">
        <button class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
            <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17 16l4-4m0 0l-4-4m4 4H7m6 4v1a3 3 0 01-3 3H6a3 3 0 01-3-3V7a3 3 0 013-3h4a3 3 0 013 3v1" />
            </svg>
            <span class="group-hover:text-gray-700">Logout</span>
        </button>
    </div>
</aside>
<div class="ml-auto mb-6 lg:w-[75%] xl:w-[80%] 2xl:w-[85%]">
    <div class="sticky z-10 top-0 h-14 border-b bg-white lg:py-2.5">
        <div class="px-6 flex items-center justify-between space-x-4 2xl:container">
            <h5 hidden class="text-2xl text-gray-600 font-medium lg:block">Dashboard</h5>
            <button class="w-12 h-16 -mr-2 border-r lg:hidden">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 my-auto" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" />
                </svg>
            </button>
            <div class="flex space-x-4">
                <!--search bar -->
            
                <!--/search bar -->
            </div>
        </div>
    </div>

    <p class="text-m italic font-medium text-gray-600 px-5 pt-5 ">Last Updated: <?php echo $row["updated_at"] ?> </p>

    <p class="text-m bold font-medium text-gray-600 px-5 pt-5 ">Environment Information</p>
    

    <dl class="mt-5 grid grid-cols-1 gap-5 sm:grid-cols-3 mx-5">
  
    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Temperature</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row["temperature"] . " &#176 F" ?> </dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Pressure</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row["pressure"] . " inches/hg" ?></dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Approximate Altitude</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row["altitude"] . " m" ?> </dd>
    </div>
	
	    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Humidity</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row["humidity"] . " %" ?> </dd>
    </div>
	
    </dl>

    <p class="text-m italic font-medium text-gray-600 px-5 pt-5 ">Last Updated: <?php echo $row1["updated_at"] ?> </p>

    <p class="text-m bold font-medium text-gray-600 px-5 pt-5 ">Battery Information</p>

   <dl class="mt-5 grid grid-cols-1 gap-5 sm:grid-cols-3 mx-5">
  
    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Battery Temperature</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["battery_temperature"] . " &#176 C " ?>  </dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Battery Percentage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["battery_soc"] . " %" ?> </dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Battery Voltage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["battery_voltage"] . " V" ?> </dd>
    </div>

  <!--
	
	<div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Battery Amperage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["battery_charging_amps"] . " A" ?> </dd>
    </div>

  -->

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Controller Temperature</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["controller_temperature"] . " &#176 C " ?> </dd>
    </div>

      <!--

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Load Voltage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["load_voltage"] . " V" ?> </dd>
    </div>


    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Load Amperage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["load_amps"] . " A" ?> </dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Load Watts</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["load_watts"] . " A" ?> </dd>
    </div>

    -->

    </dl>

    <p class="text-m italic font-medium text-gray-600 px-5 pt-5 ">Last Updated: <?php echo $row1["updated_at"] ?> </p>

    <p class="text-m bold font-medium text-gray-600 px-5 pt-5 ">Panel Information</p>

    <dl class="mt-5 grid grid-cols-1 gap-5 sm:grid-cols-3 mx-5">

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Panel Voltage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["solar_panel_voltage"] . " V" ?> </dd>
    </div>

    <!--

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Panel Amperage</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["solar_panel_amps"] . " A" ?> </dd>
    </div>

    <div class="overflow-hidden rounded-lg bg-white px-4 py-5 shadow sm:p-6 border border-gray-300">
      <dt class="truncate text-sm font-medium text-gray-500">Panel Watts</dt>
      <dd class="mt-1 text-3xl font-semibold tracking-tight text-gray-900"> <?php echo $row1["solar_panel_watts"] . " W" ?> </dd>
    </div>

     -->
	
    </dl>
  
  <p class="text-m font-medium text-gray-600 px-5 py-7 ">Welcome to the remote dashboard and control center for the 2023 Lafayette ECE Senior Design Project!</p>
  
  <dt class=" text-m font-medium text-gray-600 px-5 ">This page represents the sensor data retrieved from the RAPSCES (Remotely Accessible Portable Solar Charging Evaluation System).</dt>
  
</div>
</div>