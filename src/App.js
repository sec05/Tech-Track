import React, { useState } from "react";
import { BrowserRouter as Router, Route, Routes, Link } from "react-router-dom";
import { Button, Card, CardContent, Select, MenuItem, Box, Typography } from "@mui/material";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from "recharts";

// Sample data for the chart
const data = [
  { year: "2020", usage: 40 },
  { year: "2021", usage: 55 },
  { year: "2022", usage: 70 },
  { year: "2023", usage: 85 },
  { year: "2024", usage: 90 },
];

const techOptions = ["AI", "Blockchain", "Cloud Computing", "Cybersecurity", "Web Development"];

// TopTrends Component
function TopTrends() {
  const trends = [
    { headline: "AI Usage Soars", detail: "90% increase in AI adoption in 2024" },
    { headline: "Blockchain Gains Traction", detail: "70% more projects adopt blockchain tech" },
    { headline: "Cloud Computing Reigns", detail: "95% of companies use cloud infrastructure" },
    { headline: "Cybersecurity Demand Spikes", detail: "Security job postings up by 60%" },
    { headline: "Web Dev Frameworks Evolve", detail: "React continues to dominate frontend tech" },
  ];

  const randomTrend = trends[Math.floor(Math.random() * trends.length)];

  return (
    <Card sx={{ mb: 2, bgcolor: "black", color: "yellow" }}>
      <CardContent>
        <Typography variant="h6">Today's Top Trend</Typography>
        <Typography variant="subtitle1" sx={{ fontWeight: "bold" }}>
          {randomTrend.headline}
        </Typography>
        <Typography variant="body2">{randomTrend.detail}</Typography>
      </CardContent>
    </Card>
  );
}

// Home Component
function Home() {
  return (
    <div style={{ backgroundColor: "yellow", height: "100vh", display: "flex", flexDirection: "column", alignItems: "center", justifyContent: "center" }}>
      <Typography variant="h2" sx={{ color: "black", fontWeight: "bold" }}>
        Tech Track
      </Typography>
      <Typography variant="h5" sx={{ color: "black" }}>
        Circa 2025
      </Typography>
      <Button variant="contained" sx={{ mt: 3, bgcolor: "black", color: "yellow" }} component={Link} to="/features">
        Explore Features
      </Button>
    </div>
  );
}

// SearchBar Component
function SearchBar({ selectedTech, setSelectedTech, selectedYear, setSelectedYear, onSearch }) {
  return (
    <Card
      sx={{
        bgcolor: "white",
        color: "black",
        borderRadius: "10px",
        mt: 3,
        mb: 3, // added bottom margin to create space between the cards
      }}
    >
      
      <CardContent>
        <Select
          value={selectedTech}
          onChange={(e) => setSelectedTech(e.target.value)}
          displayEmpty
          fullWidth
          sx={{
            bgcolor: "white",
            borderRadius: "5px",
            color: "black",
            mb: 2,
            // removed border line here
          }}
        >
          <MenuItem value="" disabled>Select a Technology</MenuItem>
          {techOptions.map((tech) => (
            <MenuItem key={tech} value={tech}>{tech}</MenuItem>
          ))}
        </Select>

        <Box sx={{ display: "flex", gap: 1 }}>
          <Select
            value={selectedYear}
            onChange={(e) => setSelectedYear(e.target.value)}
            displayEmpty
            fullWidth
            sx={{
              bgcolor: "white",
              borderRadius: "5px",
              color: "black",
              // removed border line here as well
            }}
          >
            <MenuItem value="" disabled>Select Year</MenuItem>
            {data.map((entry) => (
              <MenuItem key={entry.year} value={entry.year}>{entry.year}</MenuItem>
            ))}
          </Select>

          <Button
            variant="contained"
            sx={{ bgcolor: "yellow", color: "black", minWidth: "100px" }}
            onClick={onSearch}
            disabled={!selectedTech || !selectedYear}
          >
            Search
          </Button>
        </Box>
      </CardContent>
    </Card>
  );
}

// TechChart Component
function TechChart() {
  return (
    <Card
      sx={{
        p: 2,
        mb: 3,
        bgcolor: "white",
        color: "black",
        border: "2px solid black", // added border
        borderRadius: "10px",
      }}
    >
      <CardContent>
        <Typography variant="h6" gutterBottom>Technology Trend Analysis</Typography>
        <ResponsiveContainer width="100%" height={350}>
          <LineChart data={data}>
            <CartesianGrid stroke="black" />
            <XAxis dataKey="year" stroke="black" />
            <YAxis stroke="black" />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="usage" stroke="black" strokeWidth={2} dot={{ r: 5, fill: "yellow" }} />
          </LineChart>
        </ResponsiveContainer>
      </CardContent>
    </Card>
  );
}

// Features Component
function Features() {
  const [selectedTech, setSelectedTech] = useState("");
  const [selectedYear, setSelectedYear] = useState("");
  const [showChart, setShowChart] = useState(false);

  return (
    <div style={{ maxWidth: "600px", margin: "auto", padding: "20px", background: "#ffffff", color: "black" }}>
      <Typography 
        variant="h4" 
        gutterBottom 
        sx={{ 
          color: "black", 
          textAlign: "center", 
          fontWeight: 'bold', 
        }}
      >
       Tech Track 
        <span style={{ fontSize: "0.3em", fontWeight: "normal" }}>®</span> {/* Registered Trademark symbol */}

      </Typography>
      <TopTrends />
      <SearchBar
        selectedTech={selectedTech}
        setSelectedTech={setSelectedTech}
        selectedYear={selectedYear}
        setSelectedYear={setSelectedYear}
        onSearch={() => setShowChart(true)}
      />
      {showChart && <TechChart />}
    </div>
  );
}


// App Component
function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/features" element={<Features />} />
      </Routes>

      {/* Fun Tech Track Footer */}
      <Box
        sx={{
          mt: 4,
          height: "80px",
          width: "100%",
          position: "relative",
          backgroundColor: "black",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
        }}
      >
        <Box
          sx={{
            width: "80%",
            height: "4px",
            backgroundColor: "yellow",
            borderRadius: "2px",
          }}
        ></Box>

        <img
          src="https://media.tenor.com/EJQvxUMk__MAAAAj/runner.gif"
          alt="Running Guy"
          style={{
            position: "absolute",
            bottom: "52px",
            left: "50%",
            height: "60px",
          }}
        />
      </Box>
    </Router>
  );
}

export default App;
