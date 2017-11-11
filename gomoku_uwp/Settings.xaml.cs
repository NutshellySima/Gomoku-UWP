using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace gomoku_uwp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Settings : Page
    {
        Windows.Storage.ApplicationDataContainer localSettings = Windows.Storage.ApplicationData.Current.LocalSettings;
        public Settings()
        {
            this.InitializeComponent();
            initSettings();
        }
        public void initSettings()
        {
            Object valueMode = localSettings.Values["mode"];
            Object valueBlack = localSettings.Values["black"];
            Object valueWhite = localSettings.Values["white"];
            if (valueMode == null||valueBlack==null||valueWhite==null)
            {
                localSettings.Values["mode"] = "hard";
                localSettings.Values["black"] = "computer";
                localSettings.Values["white"] = "human";
            }
            else
            {
                switch(localSettings.Values["mode"])
                {
                    case "hard":
                        Mode_Hard.IsChecked = true;
                        break;
                    case "easy":
                        Mode_Easy.IsChecked = true;
                        break;
                }
                switch (localSettings.Values["black"])
                {
                    case "computer":
                        Black_Computer.IsChecked = true;
                        break;
                    case "human":
                        Black_Human.IsChecked = true;
                        break;
                }
                switch (localSettings.Values["white"])
                {
                    case "computer":
                        White_Computer.IsChecked = true;
                        break;
                    case "human":
                        White_Human.IsChecked = true;
                        break;
                }
            }
        }

        private void B_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton rb = sender as RadioButton;
            if (rb != null)
            {
                string Name = rb.Tag.ToString();
                switch (Name)
                {
                    case "Hard":
                        localSettings.Values["mode"] = "hard";

                        break;
                    case "Easy":
                        localSettings.Values["mode"] = "easy";
                        break;
                    case "ComputerWhite":
                        localSettings.Values["white"] = "computer";
                        break;
                    case "HumanWhite":
                        localSettings.Values["white"] = "human";
                        break;
                    case "ComputerBlack":
                        localSettings.Values["black"] = "computer";
                        break;
                    case "HumanBlack":
                        localSettings.Values["black"] = "human";
                        break;
                }
            }
        }
    }
}
