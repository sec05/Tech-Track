import React from "react";
import { Typography, Box } from "@mui/material";

function Home() {
  return (
    <Box
      sx={{
        height: "100vh",
        display: "flex",
        flexDirection: "column",
        alignItems: "center",
        justifyContent: "center",
        backgroundColor: "yellow",
        color: "black",
        textAlign: "center",
      }}
    >
      <Typography variant="h2" fontWeight="bold">
        Tech Track
      </Typography>
      <Typography variant="h5">circa 2025</Typography>
    </Box>
  );
}

export default Home;
