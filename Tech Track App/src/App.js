import React, { useState, useEffect } from "react";
import {
  BrowserRouter as Router,
  Route,
  Routes,
  Link,
} from "react-router-dom";
import {
  Button,
  Card,
  CardContent,
  Select,
  MenuItem,
  Box,
  Typography,
} from "@mui/material";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer,
} from "recharts";



const predictionData = [
  { predictedUsage: 9.0 }
];

const companyOptions = [
  "Google", "GitHub", "Reddit", "All"
];

const languageOptions = [
  "Python", "Java", "C", "C++", "JavaScript", "HTML", "CSS", "Go", "Ruby", "PHP"
];

// TopTrends Component
function TopTrends() {
  const trends = [
    { headline: "AI Usage Soars", detail: "90% increase in AI adoption in 2024" },
    { headline: "Blockchain Gains Traction", detail: "70% more projects adopt blockchain tech" },
    { headline: "Cloud Computing Reigns", detail: "95% of companies use cloud infrastructure" },
    { headline: "Cybersecurity Demand Spikes", detail: "Security job postings up by 60%" },
    { headline: "Web Dev Frameworks Evolve", detail: "React continues to dominate frontend tech" },
  ];

  const { headline, detail } = trends[Math.floor(Math.random() * trends.length)];

  return (
    <Card sx={{ mb: 2, bgcolor: "black", color: "yellow" }}>
      <CardContent>
        <Typography variant="h6">Today's Top Trend</Typography>
        <Typography variant="subtitle1" sx={{ fontWeight: "bold" }}>{headline}</Typography>
        <Typography variant="body2">{detail}</Typography>
      </CardContent>
    </Card>
  );
}

// Home Component
function Home() {
  return (
    <Box
      sx={{
        backgroundColor: "yellow",
        height: "100vh",
        display: "flex",
        flexDirection: "column",
        alignItems: "center",
        justifyContent: "center",
      }}
    >
      <Typography variant="h2" sx={{ color: "black", fontWeight: "bold" }}>
        Tech Track
      </Typography>
      <Typography variant="h5" sx={{ color: "black" }}>
        Circa 2025
      </Typography>
      <Button
        variant="contained"
        sx={{ mt: 3, bgcolor: "black", color: "yellow" }}
        component={Link}
        to="/features"
      >
        Explore Features
      </Button>
    </Box>
  );
}

// SearchBar Component
function SearchBar({ selectedCompany, setSelectedCompany, selectedLanguage, setSelectedLanguage, onSearch }) {
  return (
    <Card sx={{ bgcolor: "white", color: "black", borderRadius: 2, my: 3 }}>
      <CardContent>
        <Select
          value={selectedCompany}
          onChange={(e) => setSelectedCompany(e.target.value)}
          displayEmpty
          fullWidth
          sx={{ mb: 2, borderRadius: 1 }}
        >
          <MenuItem value="" disabled>Select Company</MenuItem>
          {companyOptions.map((company) => (
            <MenuItem key={company} value={company}>{company}</MenuItem>
          ))}
        </Select>

        <Box sx={{ display: "flex", gap: 1 }}>
          <Select
            value={selectedLanguage}
            onChange={(e) => setSelectedLanguage(e.target.value)}
            displayEmpty
            fullWidth
            sx={{ borderRadius: 1 }}
          >
            <MenuItem value="" disabled>Select Language</MenuItem>
            {languageOptions.map((lang) => (
              <MenuItem key={lang} value={lang}>{lang}</MenuItem>
            ))}
          </Select>

          <Button
            variant="contained"
            sx={{ bgcolor: "yellow", color: "black", minWidth: "100px" }}
            onClick={onSearch}
            disabled={!selectedCompany || !selectedLanguage}
          >
            Search
          </Button>
        </Box>
      </CardContent>
    </Card>
  );
}

// TechChart Component (Refactored)
function TechChart({ company, language }) {
  const [chartData, setChartData] = useState([]);

  useEffect(() => {
    if (!company || !language) return;

    const fetchData = async () => {
      try {
        const lang = language.toLowerCase();
        const comp = company.toLowerCase();
        const response = await fetch(`http://localhost:80/${comp}/${lang}`);
        const data = await response.json();
        const formatted = data.times.map((time, index) => ({
          time: time.replace("+1", ""),
          value: data.values[index],
        }));
        setChartData(formatted);
      } catch (err) {
        alert("Error fetching chart data: " + err.message);
      }
    };

    fetchData();
  }, [company, language]);

  return (
    <Card sx={{ p: 2, mb: 3, bgcolor: "white", color: "black", border: "2px solid black", borderRadius: 2 }}>
      <CardContent>
        <Typography variant="h6" gutterBottom>Technology Trend Analysis</Typography>
        <ResponsiveContainer width="100%" height={350}>
          <LineChart data={chartData}>
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="time" />
            <YAxis />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="value" stroke="#facc15" />
          </LineChart>
        </ResponsiveContainer>
      </CardContent>
    </Card>
  );
}


// PredictionHighlight Component
function PredictionHighlight() {
  const { predictedUsage } = predictionData[0];

  return (
    <Card sx={{ bgcolor: "black", color: "yellow", p: 2, borderRadius: 2 }}>
      <CardContent>
        <Typography variant="subtitle1" gutterBottom>📈 Predicted Trend Peak</Typography>
        <Typography variant="body2">
          By 2025, predicted usage is expected to reach <strong>{predictedUsage}%</strong>.
        </Typography>
      </CardContent>
    </Card>
  );
}

// Features Component
function Features() {
  const [selectedCompany, setSelectedCompany] = useState("");
  const [selectedLanguage, setSelectedLanguage] = useState("");
  const [showChart, setShowChart] = useState(false);

  return (
    <Box sx={{ maxWidth: 600, mx: "auto", p: 3, background: "#fff", color: "black" }}>
      <Typography variant="h4" gutterBottom textAlign="center" fontWeight="bold">
        Tech Track <span style={{ fontSize: "0.3em", fontWeight: "normal", marginLeft: "-10px" }}>®</span>
      </Typography>

      <TopTrends />

      <SearchBar
        selectedCompany={selectedCompany}
        setSelectedCompany={setSelectedCompany}
        selectedLanguage={selectedLanguage}
        setSelectedLanguage={setSelectedLanguage}
        onSearch={() => setShowChart(true)}
      />

      {showChart && (
        <>
           <TechChart company={selectedCompany} language={selectedLanguage} />
          <PredictionHighlight />
        </>
      )}
    </Box>
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

      <Box
        sx={{
          mt: 4,
          height: "80px",
          width: "100%",
          backgroundColor: "black",
          position: "relative",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
        }}
      >
        <Box sx={{ width: "80%", height: "4px", backgroundColor: "yellow", borderRadius: "2px" }} />
        <img
          src="https://media.tenor.com/EJQvxUMk__MAAAAj/runner.gif"
          alt="Running Guy"
          style={{ position: "absolute", bottom: "52px", left: "50%", height: "60px" }}
        />
      </Box>
    </Router>
  );
}

export default App;
