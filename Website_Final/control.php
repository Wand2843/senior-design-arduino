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
$result = $conn->query($sql);
$row = $result->fetch_assoc();

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

    $arduino_hour = (int)$arduino_hour;
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
            <a href="#" title="home"></a>
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
                <a href="/dashboard.php" aria-label="dashboard" class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
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
                <a href="#" class="relative px-4 py-3 flex items-center space-x-4 rounded-xl text-white bg-gradient-to-r from-sky-600 to-cyan-400">
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
            <h5 hidden class="text-2xl text-gray-600 font-medium lg:block">Control Center</h5>
            <button class="w-12 h-14 -mr-2 border-r lg:hidden">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 my-auto" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" />
                </svg>
        </button>
        </div>
    </div>
 

<div class="flex flex-col">
  <div class="overflow-x-auto sm:-mx-6 lg:-mx-8">
    <div class="py-2 inline-block sm:px-6 lg:px-8">
      <div class="overflow-hidden">
        <table class="min-w-full">
          <thead class="bg-white border-b">
            <tr>
              <th scope="col" class="text-sm font-bold text-gray-900 px-6 py-4 text-left">
                Control Algorithm
              </th>
              <th scope="col" class="text-sm font-bold text-gray-900 px-6 py-4 text-left">
                Description
              </th>
            </tr>
          </thead>
          <tbody>
            <tr class="bg-gray-100 border-b">
              <td class="px-6 py-4 text-sm font-extralight text-gray-900">Manual Control</td>
              <td class="text-sm text-gray-900 font-light px-6 py-4">
                This mode puts complete control on the system. The user will be able to use controls on the system to manually adjust the position of the solar panel.
              </td>
            </tr>
            <tr class="bg-white border-b">
              <td class="px-6 py-4 text-sm font-extralight text-gray-900">Schedule Control</td>
              <td class="text-sm text-gray-900 font-light px-6 py-4">
                The schedule control will place the system in a mode where it will follow a schedule which we have specified on the system. This data uses the seasonal position of the sun to direct the solar panel to the optimal position based on this data. In this mode, the system will move to this position and only move again when a substantial distance is required.
              </td>
            </tr>
            <tr class="bg-gray-100 border-b">
              <td class="px-6 py-4 text-sm font-extralight text-gray-900">Automatic Control</td>
              <td colspan="2" class="text-sm text-gray-900 font-light px-6 py-4">
                This mode places the system in complete automatic control. The system uses photodiodes to measure and compare light difference to adjust the panel to the optimal light positon at a certain interval. Once placed in this mode, the system will operate autonomously.
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</div>

    
<!-- component -->
<!-- entire page -->
  <!-- tabs -->

  <div class="px-10 align-middle pt-10">
  <form method="POST">
  <div class="bg-gray-200 rounded-xl px-3 py-3"> 
    <div>
      <input type="radio" name="options" value="manual" id="1" class="peer hidden" checked />
      <label
        for="1"
        class="block cursor-pointer select-none rounded-xl p-2 text-center peer-checked:bg-blue-500 peer-checked:font-bold peer-checked:text-white peer-checked:bg-gradient-to-r from-sky-600 to-cyan-400"
        >Manual Control</label
      >
    </div>

    <div>
      <input type="radio" name="options" value="schedule" id="2" class="peer hidden" />
      <label
        for="2"
        class="block cursor-pointer select-none rounded-xl p-2 text-center peer-checked:bg-blue-500 peer-checked:font-bold peer-checked:text-white peer-checked:bg-gradient-to-r from-sky-600 to-cyan-400""
        >Schedule Control</label>
    </div>

    <div>
      <input type="radio" name="options" value="automatic" id="3" class="peer hidden" />
      <label
        for="3"
        class="block cursor-pointer select-none rounded-xl p-2 text-center peer-checked:bg-blue-500 peer-checked:font-bold peer-checked:text-white peer-checked:bg-gradient-to-r from-sky-600 to-cyan-400"
        >Automatic Control</label
      >
    </div>
  </div>


<?php

if(isset($_POST['submit'])) {

  if(isset($_POST['options'])) {

    $selectedMode = $_POST['options'];

    if($selectedMode == "manual") {
      $sql1 = "DELETE FROM control";
      $commit2 = $conn->query($sql1);
      $sql2 = "INSERT INTO control (manual, schedule, automatic) VALUES (1,0,0)";
      $commit2 = $conn->query($sql2);
    }

    else if($selectedMode == "schedule") {
      $sql1 = "DELETE FROM control";
      $commit2 = $conn->query($sql1);
      $sql2 = "INSERT INTO control (manual, schedule, automatic) VALUES (0,1,0)";
      $commit2 = $conn->query($sql2);
    }

    else if($selectedMode == "automatic") {
      $sql1 = "DELETE FROM control";
      $commit2 = $conn->query($sql1);
      $sql2 = "INSERT INTO control (manual, schedule, automatic) VALUES (0,0,1)";
      $commit2 = $conn->query($sql2);
  
    }

  }
  
}

?>

<div class="flex space-x-2 justify-center pt-10">
  <input
    type="submit"
    name="submit"
    data-mdb-ripple="true"
    data-mdb-ripple-color="light"
    class="inline-block px-6 py-2.5 bg-blue-600 text-white font-medium text-xs leading-tight uppercase rounded shadow-md hover:bg-blue-700 hover:shadow-lg focus:bg-blue-700 focus:shadow-lg focus:outline-none focus:ring-0 active:bg-blue-800 active:shadow-lg transition duration-150 ease-in-out"
  >
</input>

</div>
</form>

<?php

$sql3 = "SELECT manual,schedule,automatic FROM control";
$result = $conn->query($sql3);
$row = $result->fetch_assoc();

if ($row['manual'] == 1) {
  echo "<p class='text-center px-6 py-4 text-sm font-extralight text-gray-900'>Current System Operation: Manual Control</p>"; 
}
else if ($row['schedule'] == 1) {
  echo "<p class='text-center px-6 py-4 text-sm font-extralight text-gray-900'>Current System Operation: Schedule Control</p>"; 
}
else if ($row['automatic'] == 1) {
  echo "<p class='text-center px-6 py-4 text-sm font-extralight text-gray-900'>Current System Operation: Automatic Control</p>"; 
}

?>

</div>
</div>